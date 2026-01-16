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
  pets.Load();
  
  Serial.println("Application Prête !");
  screen.Refresh();
  screen.DrawDashboard("Init...", 0, pets.getName()); // Ecran initialisation
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
         screen.DrawDashboard(currentTime, level, pets.getName());
         lastUIUpdate = millis();
      }
      break;

    case STATE_MENU_MAIN:
      HandleMenuMain(evt);
      break;
      
    case STATE_MENU_SCHEDULE:
      HandleMenuSchedule(evt);
      break;

    case STATE_EDIT_MEAL_TIME:
      HandleEditMealTime(evt, true, menuIndex);
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
    
    case STATE_SELECT_SPECIES :
      HandleSpeciesSelect(evt);
      break;

    case STATE_SHOW_SUMMARY:
      HandleAnimalSummary(evt);
      break;

    case STATE_SHOW_HOURS:
      HandleHourSummary(evt);
      break;

    case STATE_EDIT_FIELD:
      // On décide si le clavier est numérique ou alpha selon la cible
      bool isNum = (menuIndex != 0); //menuindex != 0
      HandleKeyboard(evt, isNum);
      break;
  }

  // Gestion Automatique (Horaires)
  distributor.CheckAutoFeed(currentTime, &motor, pets.getSpecies()->getRationCoef());
}

// --- Logique des États ---

void Application::HandleDashboard(InputEvent evt) {
  // N'importe quel bouton ouvre le menu
  if (evt != EVT_NONE) {
    currentState = STATE_MENU_MAIN;
    menuIndex = 0; // Reset sélection
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
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
      switch (menuIndex) {
        case 0:
          pets.setName(tempInputString);
          break;
        case 2:
          pets.setAge(tempInputString.toInt());
          break;
        case 3:
          pets.setWeight(tempInputString.toInt());
          break;
        case 4:
          pets.setHeight(tempInputString.toInt());
          break;
      }

      // Sauvegarde en mémoire ROM
      pets.Save();
      
      // Retour menu
      currentState = STATE_MENU_ANIMAL;
      screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
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
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
    return;
  }

  // Redessiner clavier
  String titre = "Valeur";
  if (menuIndex == 0) titre = "Nom";
  else if (menuIndex == 2) titre = "Age";
  else if (menuIndex == 3) titre = "Poids";
  else if (menuIndex == 4) titre = "Taille";
  screen.DrawKeyboard(titre, tempInputString, keyboardIndex, isNumeric);
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
    screen.DrawDashboard("Wait...", 0, pets.getName());
    return;
  }
  else if (evt == EVT_ENTER) {
    // Validation du choix
    switch (menuIndex) {
      case 0: // Horaires
        currentState = STATE_MENU_SCHEDULE;
        menuIndex = 0;
        screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
        return;

      case 1: // Gestion Animal
        currentState = STATE_MENU_ANIMAL;
        menuIndex = 0;
        tempInputString = ""; // Vide le champ
        screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, 0);
        return; 

      case 2: // Distribution manuelle
        currentState = STATE_MANUAL_FEED;
        screen.DrawMenu("DISTRIBUTION", manualMenuItems, 1, 0, 0);
        return;
    }
  }

  // Si on est toujours dans le menu, on redessine pour mettre à jour la surbrillance
  if (currentState == STATE_MENU_MAIN) {
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
  }
}

void Application::HandleMenuSchedule(InputEvent evt) {
  if (evt == EVT_NONE) return;

  if (evt == EVT_DOWN) {
    menuIndex++;
    if (menuIndex > 3) menuIndex = 0; // Bouclage
  } 
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = 3;
  } 
  else if (evt == EVT_BACK) {
    // Retour au menu principal
    currentState = STATE_MENU_MAIN;
    menuIndex = 0; // On se remet sur "Horaires"
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
    return;
  }
  else if (evt == EVT_ENTER) {
    tempInputString = ""; // Reset buffer clavier
    keyboardIndex = 0;    // Reset curseur clavier

    switch (menuIndex) {
      case 0: // Heure 1
      case 1: // Heure 2
      case 2: // Heure 3
        
        currentState = STATE_EDIT_MEAL_TIME;
        HandleEditMealTime(evt, true, menuIndex);
        break;
      case 3: // Fiche horaire
        currentState = STATE_SHOW_HOURS;
        screen.DrawHourSummary(distributor.getMealTime(0), distributor.getMealTime(1), distributor.getMealTime(2));
        break;
    }
    return;
  }

  if (currentState == STATE_MENU_SCHEDULE && evt != EVT_NONE) {
    screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
  }
}

void Application::HandleEditMealTime(InputEvent evt, bool isNumeric, int index) {
  if (evt == EVT_NONE) return;
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
    screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
    return;
  }
  else if (evt == EVT_ENTER) {
    char key = screen.GetKeyChar(keyboardIndex, true);
    
    if (key == '<') { 
       if (tempInputString.length() > 0) tempInputString.remove(tempInputString.length() - 1);
    }
    else if (key == '>')
    { // VALIDER
       // Validation du format HHMM
      try
      {
        if (tempInputString.length() == 4)
        {
          String hh = tempInputString.substring(0, 2);
          String mm = tempInputString.substring(2, 4);
          
          // Vérif basique
          if (hh.toInt() < 24 && mm.toInt() < 60)
          {
            String formattedTime = hh + ":" + mm;
            distributor.setMealTime(index, formattedTime);
            distributor.Save();
            
            currentState = STATE_MENU_SCHEDULE;
            screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
            return;
          } 
          else
          {
            throw DistribException(2, "Heure Invalide !");
          }
        }
        else
        {
          throw DistribException(1, "Format HHMM SVP");
        }
      }
      catch (DistribException ex)
      {
        switch (ex.exType)
        {
          case 2:
          case 1:
            screen.ShowMessage(ex.exMessage);
            delay(1000);
            tempInputString = ""; // Reset
            break;
          
          default:
            break;
        }
      }
    }
    else { // Chiffre
      if (tempInputString.length() < 4){
        tempInputString += key;
      }
    }
  }

  screen.DrawKeyboard("Repas " + (String)(index+1), tempInputString, keyboardIndex, true);
}

void Application::HandleMenuAnimal(InputEvent evt) {
  int itemCount = 7; // Nombre d'items dans le menu Animal
  int maxVisible = 4;

  if (evt == EVT_DOWN) {
    menuIndex++; 
    if (menuIndex > 6) {
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
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
    return;
  }
  else if (evt == EVT_ENTER) {
    // Choix de l'élément à modifier
    tempInputString = ""; // Reset buffer clavier
    keyboardIndex = 0;    // Reset curseur clavier

    switch (menuIndex) {
      case 0: // Nom
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Nom de l'animal", pets.getName(), 0, false); // false = Alpha
        break;
      case 1: // Especes 
        currentState = STATE_SELECT_SPECIES;
        menuIndex = 0;
        screen.DrawMenu("Choix Especes", speciesItems, 5, menuIndex, 0);
        break;
      case 2: // Age
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Age", String(pets.getAge()), 0, true); // true = Num
        break;
      case 3: // Poids
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Poids (kg)", String(pets.getWeight()), 0, true);
        break;
      case 4: // Taille
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Taille (cm)", String(pets.getHeight()), 0, true);
        break;
      case 5: // Comportement
        currentState = STATE_SELECT_BEHAVIOR;
        menuIndex = 0;
        screen.DrawMenu("Comportement", behaviorItems, 2, 0, 0);
        break;
      case 6: // Voir Fiche
        currentState = STATE_SHOW_SUMMARY;
        screen.DrawAnimalSummary(pets.getName(), pets.getSpecies()->getSPName(), pets.getAge(), pets.getWeight(), pets.getHeight(), pets.getBehavior());
        break;
    }
    return;
  }

  if (currentState == STATE_MENU_ANIMAL && evt != EVT_NONE) {
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, itemCount, menuIndex, menuScrollOffset);
  }
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
    if (menuIndex == 0) pets.setBehavior("Calme");
    else pets.setBehavior("Agressif");

    // Sauvegarde en mémoire ROM
    pets.Save();

    // Retour
    currentState = STATE_MENU_ANIMAL;
    menuIndex = 5; // On revient sur la ligne "Comportement"
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
    return;
  }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_ANIMAL;
    menuIndex = 5;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
    return;
  }

  if (currentState == STATE_SELECT_BEHAVIOR && evt != EVT_NONE) {
    screen.DrawMenu("COMPORTEMENT", behaviorItems, 2, menuIndex, 0);
  }
}

void Application::HandleSpeciesSelect(InputEvent evt){
  int itemCount = 5; // Nombre d'items dans le menu Espece
  int maxVisible = 4;

  if (evt == EVT_DOWN) {
    menuIndex++; 
    if (menuIndex > 4) {
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
    menuIndex = 1; // On se remet sur "Espece"
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, 0);;
    return;
  }
  else if (evt == EVT_ENTER) {
    // Choix de l'élément à modifier
    switch (menuIndex) {
      case 0: // Chien
        pets.setSpecies(new Dog());
        break;
      case 1: // Chat
        pets.setSpecies(new Cat());
        break;
      case 2: // Baleine
        pets.setSpecies(new Whale());
        break;
      case 3: // Cheval
        pets.setSpecies(new Horse());
        break;
      case 4: // Souris
        pets.setSpecies(new Mouse()); 
        break;
    }
    // Sauvegarde en mémoire ROM
    pets.Save();

    // Retour
    currentState = STATE_MENU_ANIMAL;
    menuIndex = 1; // On revient sur la ligne "Espece"
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, 0);
    return;
  }

  if (currentState == STATE_SELECT_SPECIES && evt != EVT_NONE) {
    screen.DrawMenu("Choix Especes", speciesItems, 5, menuIndex, menuScrollOffset);
  }
}

void Application::HandleAnimalSummary(InputEvent evt) {
  if (evt == EVT_BACK || evt == EVT_ENTER) {
    currentState = STATE_MENU_ANIMAL;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, 3);
  }
}

void Application::HandleHourSummary(InputEvent evt) {
  if (evt == EVT_BACK || evt == EVT_ENTER) {
    currentState = STATE_MENU_SCHEDULE;
    screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
  }
}

void Application::HandleManualFeed(InputEvent evt) {
  if (evt == EVT_ENTER) {
    // Feedback visuel immédiat
    Serial.println("Distribution en cours...");
    screen.ShowMessage("Distribution en cours...");
    
    // Action Moteur (cette fonction est bloquante quelques secondes)
    distributor.ForceFeed(&motor, pets.getSpecies()->getRationCoef());
    
    // Feedback de fin
    Serial.println("Termine !");
    screen.ShowMessage("Termine !");
    
    // On réaffiche le menu de distribution
    currentState = STATE_MANUAL_FEED;
    screen.DrawMenu("DISTRIBUTION", manualMenuItems, 1, 0, 0);
  }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_MAIN;
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
  }
}