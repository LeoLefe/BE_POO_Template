/*********************************************************************
 * @file  Apllication.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Application
 *********************************************************************/
#include "Application.h"

Application::Application() : currentState(STATE_DASHBOARD), menuIndex(0), menuScrollOffset(0), keyboardIndex(0), lastUIUpdate(0) {}
Application::~Application() {}

void Application::Init() {
  Serial.println("Init Application...");
  
  motor.Init();
  inputs.Init();
  screen.Init();
  network.Init(); 
  sensor.Init();
  distributor.Init();

  // Chargement des données sauvegardées en mémoire
  myAnimal.Load();
  
  Serial.println("Application Prête !");
  screen.Refresh();
  screen.DrawDashboard("Init...", 0, myAnimal.getName()); // Ecran initialisation
}

void Application::Run() {
  // Gestion Réseau
  network.Update();
  String currentTime = network.GetTimeString();

  // Lire les entrées
  InputEvent evt = inputs.ReadEvent();
  
  // Machine à états
  switch (currentState) {
    case STATE_DASHBOARD:
      HandleDashboard(evt);
      
      // Update ecran de base
      if (millis() - lastUIUpdate > 1000) {
         uint8_t level = sensor.GetLevelPercent();
         screen.DrawDashboard(currentTime, level, myAnimal.getName());
         lastUIUpdate = millis();
      }
      break;

    case STATE_MENU_MAIN:
      HandleMenuMain(evt);
      break;
      
    case STATE_MENU_SCHEDULE:
      HandleMenuSchedule(evt);
      break;
      
    case STATE_EDIT_MEAL_COUNT:
      HandleEditMealCount(evt);
      break;

    case STATE_EDIT_MEAL_TIME:
      HandleEditMealTime(evt, true);
      break;

    case STATE_MANUAL_FEED:
      HandleManualFeed(evt);
      break;

    case STATE_MENU_ANIMAL:
      HandleMenuAnimal(evt);
      break;

    case STATE_SELECT_BEHAVIOR:
      HandleBehaviorSelect(evt);
      break;

    case STATE_SHOW_SUMMARY:
      HandleAnimalSummary(evt);
      break;

    case STATE_EDIT_FIELD:
      // On décide si le clavier est numérique ou alpha selon la cible
      bool isNum = (currentEditTarget != TARGET_NAME);
      HandleKeyboard(evt, isNum);
      break;
  }

  // Gestion Automatique (Horaires)
  distributor.CheckAutoFeed(currentTime, &motor);
}

// --- Logique des États ---

void Application::HandleDashboard(InputEvent evt) {
  // N'importe quel bouton ouvre le menu
  if (evt != EVT_NONE) {
    currentState = STATE_MENU_MAIN;
    menuIndex = 0; // Reset sélection
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
  }
}

void Application::HandleMenuMain(InputEvent evt) {
  if (evt == EVT_NONE) return;

  if (evt == EVT_DOWN) {
    menuIndex++;
    if (menuIndex > 2) menuIndex = 0; // Bouclage
  } 
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = 2;
  } 
  else if (evt == EVT_BACK) {
    currentState = STATE_DASHBOARD;
    screen.Refresh();
    screen.DrawDashboard("Wait...", 0, myAnimal.getName());
    return;
  }
  else if (evt == EVT_ENTER) {
    // Validation du choix
    switch (menuIndex) {
      case 0: // Horaires
        currentState = STATE_MENU_SCHEDULE;
        menuIndex = 0;
        //screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 6, menuIndex, 0);
        //screen.ShowMessage("Horaires: 08h & 19h"); // Simplifié pour l'instant
        return;

      case 1: // Gestion Animal
        currentState = STATE_MENU_ANIMAL;
        menuIndex = 0;
        tempInputString = ""; // Vide le champ
        screen.DrawMenu("MENU ANIMAL", animalMenuItems, 6, menuIndex, 0);
        return; 

      case 2: // Distribution manuelle
        currentState = STATE_MANUAL_FEED;
        screen.DrawMenu("DISTRIBUTION", manualMenuItems, 1, 0);
        return;
    }
  }

  // Si on est toujours dans le menu, on redessine pour mettre à jour la surbrillance
  if (currentState == STATE_MENU_MAIN) {
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
  }
}

void Application::HandleMenuSchedule(InputEvent evt) {
  
  // 1. Préparation des données du menu
  int count = distributor.getMealCount();
  
  // Ligne 0 : Le nombre de repas
  sprintf(menuScheduleItemsBuffer[0], "Nb Repas : %d", count);
  menuScheduleItems[0] = menuScheduleItemsBuffer[0];

  // Lignes 1 à N : Les heures
  for (int i = 0; i < count; i++) {
    String timeStr = distributor.getMealTime(i); // Renvoie HH:MM
    sprintf(menuScheduleItemsBuffer[i+1], "Repas %d : %s", i+1, timeStr.c_str());
    menuScheduleItems[i+1] = menuScheduleItemsBuffer[i+1];
  }
  
  int totalItems = count + 1; // +1 pour la ligne "Nb Repas"

  // 2. Gestion Navigation
  if (evt == EVT_DOWN) {
    menuIndex++;
    // Logique scroll (reprise de ton code précédent)
    if (menuIndex >= totalItems) { 
      menuIndex = 0; 
      menuScrollOffset = 0; 
    }
    else if (menuIndex >= menuScrollOffset + 4) { 
      menuScrollOffset++; 
    }
  }
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0) { 
        menuIndex = totalItems - 1; 
        menuScrollOffset = max(0, totalItems - 4); 
    }
    else if (menuIndex < menuScrollOffset) { 
      menuScrollOffset--; 
    }
  }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_MAIN;
    menuIndex = 0; // Retour sur "Horaires"
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
    return;
  }
  else if (evt == EVT_ENTER) {
    if (menuIndex == 0) {
      // Modification du nombre de repas
      currentState = STATE_EDIT_MEAL_COUNT;
      // On affiche une petite interface simple ou on réutilise le menu
      screen.ShowMessage("Changer Nb..."); 
      // Astuce: On redessine juste le menu mais on change l'état pour capturer HAUT/BAS différemment
    } else {
      // Modification d'une heure spécifique
      selectedMealToEdit = menuIndex - 1; // Car l'index 0 est le compteur
      currentState = STATE_EDIT_MEAL_TIME;
      tempInputString = ""; // Reset buffer clavier
      keyboardIndex = 0;
      // On pré-remplit avec l'heure actuelle sans les ':' pour éditer facilement ?
      // Pour faire simple : on demande de tout retaper
      screen.DrawKeyboard("HEURE (HHMM)", "", 0, true); // true = Numérique
      return;
    }
  }

  // 3. Affichage si changement ou en boucle
  if (currentState == STATE_MENU_SCHEDULE) {
     screen.DrawMenu("HORAIRES", menuScheduleItems, totalItems, menuIndex, menuScrollOffset);
  }
}

void Application::HandleEditMealCount(InputEvent evt) {
  int currentCount = distributor.getMealCount();
  bool changed = false;

  if (evt == EVT_UP) {
    currentCount++;
    changed = true;
  } else if (evt == EVT_DOWN) {
    currentCount--;
    changed = true;
  } else if (evt == EVT_ENTER || evt == EVT_BACK) {
    // Validation et Sauvegarde
    distributor.Save();
    currentState = STATE_MENU_SCHEDULE;
    return; // La boucle principale redessinera le menu
  }

  if (changed) {
    distributor.setMealCount(currentCount); // Gère les limites 1-5 en interne
    
    // Feedback visuel : On met à jour le texte du menu "Nb Repas" en direct
    // Pour faire propre, on peut afficher un message temporaire ou juste redessiner le menu
    // Ici, comme on est en STATE_EDIT_MEAL_COUNT, on retourne dans STATE_MENU_SCHEDULE
    // pour voir la liste s'agrandir/rétrécir instantanément ?
    // Non, restons en mode édition mais mettons à jour l'écran :
    
    screen.ShowMessage("Nb: " + String(distributor.getMealCount()));
    delay(200); // Petit délai
  }
}

void Application::HandleEditMealTime(InputEvent evt, bool isNumeric) {
  // On réutilise la logique HandleKeyboard générique, 
  // MAIS on doit intercepter la validation (ENTER -> '>') spécifique

  // Copie de la logique de navigation clavier ...
  // ... (Reprendre le code de HandleKeyboard pour UP/DOWN/BACK) ...
  // L'idéal est de modifier HandleKeyboard pour qu'il appelle une fonction de callback ou gère via l'état

  // SIMPLIFICATION : Je remets la logique ici pour être clair
  int maxKeys = screen.GetKeyCount(true); // Numérique

  if (evt == EVT_DOWN) { keyboardIndex++; if (keyboardIndex >= maxKeys) keyboardIndex = 0; }
  else if (evt == EVT_UP) { keyboardIndex--; if (keyboardIndex < 0) keyboardIndex = maxKeys - 1; }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_SCHEDULE;
    return;
  }
  else if (evt == EVT_ENTER) {
    char key = screen.GetKeyChar(keyboardIndex, true);
    
    if (key == '<') { 
       if (tempInputString.length() > 0) tempInputString.remove(tempInputString.length() - 1);
    }
    else if (key == '>') { // VALIDER
       // Validation du format HHMM
       if (tempInputString.length() == 4) {
         String hh = tempInputString.substring(0, 2);
         String mm = tempInputString.substring(2, 4);
         
         // Vérif basique
         if (hh.toInt() < 24 && mm.toInt() < 60) {
           String formattedTime = hh + ":" + mm;
           distributor.setMealTime(selectedMealToEdit, formattedTime);
           distributor.Save();
           
           currentState = STATE_MENU_SCHEDULE;
           return;
         } else {
           screen.ShowMessage("Heure Invalide !");
           delay(1000);
           tempInputString = ""; // Reset
         }
       } else {
           screen.ShowMessage("Format HHMM SVP");
           delay(1000);
       }
    }
    else { // Chiffre
       if (tempInputString.length() < 4) tempInputString += key;
    }
  }

  screen.DrawKeyboard("REPAS " + String(selectedMealToEdit + 1), tempInputString, keyboardIndex, true);
}

void Application::HandleMenuAnimal(InputEvent evt) {
  int itemCount = 6; // Nombre d'items dans le menu Animal
  int maxVisible = 4;

  if (evt == EVT_DOWN) {
    menuIndex++; 
    if (menuIndex > 5) {
      menuIndex = 0;
      menuScrollOffset = 0;
    }
    else if (menuIndex >= menuScrollOffset + maxVisible) {
      menuScrollOffset++; // On décale la fenêtre vers le bas
    }
  }
  else if (evt == EVT_UP) {
    menuIndex--; 
    if (menuIndex < 0) {
      menuIndex = itemCount - 1; // Dernier item
      menuScrollOffset = itemCount - maxVisible; 
      if (menuScrollOffset < 0) {
        menuScrollOffset = 0; // Sécurité
      }
    }
    // Si le curseur dépasse le haut de la fenêtre visible
    else if (menuIndex < menuScrollOffset) {
      menuScrollOffset--; // On décale la fenêtre vers le haut
    }
  }
  else if (evt == EVT_BACK) {
    // Retour au menu principal
    currentState = STATE_MENU_MAIN;
    menuIndex = 1; // On se remet sur "Animal"
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
    return;
  }
  else if (evt == EVT_ENTER) {
    // Choix de l'élément à modifier
    tempInputString = ""; // Reset buffer clavier
    keyboardIndex = 0;    // Reset curseur clavier

    switch (menuIndex) {
      case 0: // Nom
        currentEditTarget = TARGET_NAME;
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Nom de l'animal", myAnimal.getName(), 0, false); // false = Alpha
        break;
      case 1: // Age
        currentEditTarget = TARGET_AGE;
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Age", String(myAnimal.getAge()), 0, true); // true = Num
        break;
      case 2: // Poids
        currentEditTarget = TARGET_WEIGHT;
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Poids (kg)", String(myAnimal.getWeight()), 0, true);
        break;
      case 3: // Taille
        currentEditTarget = TARGET_HEIGHT;
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Taille (cm)", String(myAnimal.getHeight()), 0, true);
        break;
      case 4: // Comportement (Pas de clavier, mais un autre menu)
        currentState = STATE_SELECT_BEHAVIOR;
        menuIndex = 0;
        screen.DrawMenu("Comportement", behaviorItems, 2, 0);
        break;
      case 5: // Voir Fiche
        currentState = STATE_SHOW_SUMMARY;
        screen.DrawAnimalSummary(myAnimal.getName(), myAnimal.getAge(), myAnimal.getWeight(), myAnimal.getHeight(), myAnimal.getBehavior());
        break;
    }
    return;
  }

  if (currentState == STATE_MENU_ANIMAL && evt != EVT_NONE) {
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, itemCount, menuIndex, menuScrollOffset);
  }
}

void Application::HandleKeyboard(InputEvent evt, bool isNumeric) {
  if (evt == EVT_NONE) return;

  int maxKeys = screen.GetKeyCount(isNumeric);

  if (evt == EVT_DOWN) {
    keyboardIndex++;
    // Si on dépasse la fin, on revient au début (boucle)
    if (keyboardIndex >= maxKeys) keyboardIndex = 0;
  }
  // BP3 (UP) -> Va vers la gauche (Précédent)
  else if (evt == EVT_UP) {
    keyboardIndex--;
    // Si on est avant le début, on va à la fin
    if (keyboardIndex < 0) keyboardIndex = maxKeys - 1;
  }
  else if (evt == EVT_ENTER) {
    char key = screen.GetKeyChar(keyboardIndex, isNumeric);
    
    if (key == '<') { // Effacer
      if (tempInputString.length() > 0) {
        tempInputString.remove(tempInputString.length() - 1);
      }
    }
    else if (key == '>') { // OK Valider
      
      // On regarde ce qu'on était en train d'éditer
      switch (currentEditTarget) {
        case TARGET_NAME:
          myAnimal.setName(tempInputString);
          break;
        case TARGET_AGE:
          myAnimal.setAge(tempInputString.toInt());
          break;
        case TARGET_WEIGHT:
          myAnimal.setWeight(tempInputString.toInt());
          break;
        case TARGET_HEIGHT:
          myAnimal.setHeight(tempInputString.toInt());
          break;
      }

      // Sauvegarde en mémoire ROM
      myAnimal.Save();
      
      // Retour menu
      currentState = STATE_MENU_ANIMAL;
      currentEditTarget = TARGET_NONE; // Reset sécurité
      //menuIndex = 0;
      screen.DrawMenu("MENU ANIMAL", animalMenuItems, 6, menuIndex, 0);
      return;
      return;
    }
    else { // Lettre normale
      if (tempInputString.length() < 10) { // Limite taille
        tempInputString += key;
      }
    }
  }
  else if (evt == EVT_BACK) {
    // Annuler saisie
    currentState = STATE_MENU_ANIMAL;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 6, menuIndex, 0);
    return;
  }

  // Redessiner clavier
  String titre = "Valeur";
  if (currentEditTarget == TARGET_NAME) titre = "Nom";
  else if (currentEditTarget == TARGET_AGE) titre = "Age";
  else if (currentEditTarget == TARGET_WEIGHT) titre = "Poids";
  else if (currentEditTarget == TARGET_HEIGHT) titre = "Taille";
  screen.DrawKeyboard(titre, tempInputString, keyboardIndex, isNumeric);
}

void Application::HandleBehaviorSelect(InputEvent evt) {
  if (evt == EVT_DOWN) {
    menuIndex++; if (menuIndex > 1) menuIndex = 0;
  }
  else if (evt == EVT_UP) {
    menuIndex--; if (menuIndex < 0) menuIndex = 1;
  }
  else if (evt == EVT_ENTER) {
    // Sauvegarde
    if (menuIndex == 0) myAnimal.setBehavior("Calme");
    else myAnimal.setBehavior("Agressif");

    // Sauvegarde en mémoire ROM
    myAnimal.Save();

    // Retour
    currentState = STATE_MENU_ANIMAL;
    menuIndex = 4; // On revient sur la ligne "Comportement"
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 6, menuIndex, 1);
    return;
  }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_ANIMAL;
    menuIndex = 4;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 6, menuIndex, 1);
    return;
  }

  if (currentState == STATE_SELECT_BEHAVIOR && evt != EVT_NONE) {
    screen.DrawMenu("COMPORTEMENT", behaviorItems, 2, menuIndex);
  }
}

void Application::HandleAnimalSummary(InputEvent evt) {
  if (evt == EVT_BACK || evt == EVT_ENTER) {
    currentState = STATE_MENU_ANIMAL;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 6, menuIndex, 2);
  }
}

void Application::HandleManualFeed(InputEvent evt) {
  if (evt == EVT_ENTER) {
    // Feedback visuel immédiat
    Serial.println("Distribution en cours...");
    screen.ShowMessage("Distribution en cours...");
    
    // Action Moteur (cette fonction est bloquante quelques secondes)
    distributor.ForceFeed(&motor);
    
    // Feedback de fin
    Serial.println("Termine !");
    screen.ShowMessage("Termine !");
    
    // On réaffiche le menu de distribution
    currentState = STATE_MANUAL_FEED;
    screen.DrawMenu("DISTRIBUTION", manualMenuItems, 1, 0);
  }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_MAIN;
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
  }
}