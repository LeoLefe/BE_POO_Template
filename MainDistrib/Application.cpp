#include "esp32-hal.h"
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
      
      // Update écran de base
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
      // On décide si le clavier est numérique ou alphanumérique selon la cible
      bool isNum = (menuIndex != 0);
      HandleKeyboard(evt, isNum);
      break;
  }

  try
  {
    distributor.CheckAutoFeed(currentTime, &motor, pets.getSpecies()->getRationCoef(), sensor.GetLevelPercent()); // Gestion Automatique de la distribution des croquettes
  }
  catch (DistribException ex)
  {
    switch (ex.exType)
    {
      case 3:
        screen.ShowMessage(ex.exMessage);
        delay(1000);
        tempInputString = "";
        screen.Refresh();
        break;
      
      default:
        break;
    }
  }
}

// --- Logique des États ---

// Gestion de l'écran d'accueil
void Application::HandleDashboard(InputEvent evt) {
  // N'importe quel bouton ouvre le menu
  if (evt != EVT_NONE) {
    currentState = STATE_MENU_MAIN;
    menuIndex = 0; // Pointe sur le premier item du prochain menu
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
  }
}

// Gestion du clavier pour le menu animal
void Application::HandleKeyboard(InputEvent evt, bool isNumeric) {
  if (evt == EVT_NONE)
    return;

  int maxKeys = screen.GetKeyCount(isNumeric);  // Nombre de touches du clavier

  if (evt == EVT_DOWN) {  // Déplacement du curseur à droite sur le clavier
    keyboardIndex++;
    if (keyboardIndex >= maxKeys) // Si on dépasse la fin, on revient au début du clavier
      keyboardIndex = 0;
  }
  else if (evt == EVT_UP) { // Déplacement du curseur à gauche sur le clavier 
    keyboardIndex--;
    if (keyboardIndex < 0)
      keyboardIndex = maxKeys - 1;  // Si on dépasse le début, on revient à la fin du clavier
  }
  else if (evt == EVT_ENTER) {  // Appui sur une touche avec le bouton 4 (OK)
    char key = screen.GetKeyChar(keyboardIndex, isNumeric);
    
    if (key == '<') { // Si la touche choisie est "Supprimer", un caractère du champ est effacé
      if (tempInputString.length() > 0) {
        tempInputString.remove(tempInputString.length() - 1);
      }
    }
    else if (key == '>') { // Si la touche choisie est "Valider", le champ est validé
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
      
      // Retour menu animal
      currentState = STATE_MENU_ANIMAL;
      screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
      return;
    }
    else { // Si la touche choisie est un caractère, il est ajouté au champ
      if (tempInputString.length() < 10) { // Limite taille du champ
        tempInputString += key;
      }
    }
  }
  else if (evt == EVT_BACK) {
    // Annuler saisie, retour au menu animal
    currentState = STATE_MENU_ANIMAL;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
    return;
  }

  // Redessiner clavier avec titre correspondant à l'item modifié
  String titre = "Valeur";
  if (menuIndex == 0) titre = "Nom";
  else if (menuIndex == 2) titre = "Age";
  else if (menuIndex == 3) titre = "Poids";
  else if (menuIndex == 4) titre = "Taille";
  screen.DrawKeyboard(titre, tempInputString, keyboardIndex, isNumeric);
}

// Gestion du menu principal
void Application::HandleMenuMain(InputEvent evt) {
  if (evt == EVT_NONE)
    return;

  if (evt == EVT_DOWN) {
    menuIndex++;
    if (menuIndex > 2)
      menuIndex = 0; // Reviens en haut de la liste une fois le dernier élément passé
  } 
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0)
      menuIndex = 2;  // Reviens en bas de la liste une fois le premier élément passé
  } 
  else if (evt == EVT_BACK) { // Reviens au menu précédent en cas d'appui sur le bouton retour
    currentState = STATE_DASHBOARD;
    screen.Refresh();
    screen.DrawDashboard("Wait...", 0, pets.getName());
    return;
  }
  else if (evt == EVT_ENTER) {  // Va au menu suiavnt en selectionnant l'item
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
        tempInputString = ""; // Vide le champ d'entrée
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

// Gestion du menu horaires
void Application::HandleMenuSchedule(InputEvent evt) {
  if (evt == EVT_NONE)
    return;

  if (evt == EVT_DOWN) {
    menuIndex++;
    if (menuIndex > 3)
      menuIndex = 0;
  } 
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0)
      menuIndex = 3;
  } 
  else if (evt == EVT_BACK) {
    // Retour au menu principal
    currentState = STATE_MENU_MAIN;
    menuIndex = 0; // Préselection de l'item "Horaires"
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
      case 3: // Fiche heures des repas 
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

// Gestion de la modification des heures de repas
void Application::HandleEditMealTime(InputEvent evt, bool isNumeric, int index) {
  if (evt == EVT_NONE)
    return;

  int maxKeys = screen.GetKeyCount(true); // Nombre de touches du clavier numérique

  if (evt == EVT_DOWN) {  // Déplacement du curseur à droite sur le clavier 
    keyboardIndex++;
    if (keyboardIndex >= maxKeys)
      keyboardIndex = 0;
  }
  else if (evt == EVT_UP) { // Déplacement du curseur à gauche sur le clavier 
    keyboardIndex--;
    if (keyboardIndex < 0)
      keyboardIndex = maxKeys - 1;
  }
  else if (evt == EVT_BACK) { // Retour au menu horaires
    currentState = STATE_MENU_SCHEDULE;
    screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
    return;
  }
  else if (evt == EVT_ENTER) {  // Appui sur une touche avec le bouton 4 (OK)
    char key = screen.GetKeyChar(keyboardIndex, true);
    
    if (key == '<') { // Si la touche choisie est "Supprimer", un chiffre de l'heure est effacé
      if (tempInputString.length() > 0)
        tempInputString.remove(tempInputString.length() - 1);
    }
    else if (key == '>')  // Si la touche choisie est "Valider", l'heure est validée sous condition
    {
      // Validation du format HHMM
      try
      {
        if (tempInputString.length() == 4)
        {
          String hh = tempInputString.substring(0, 2);
          String mm = tempInputString.substring(2, 4);
          
          // Vérification temporelle
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
            throw DistribException(2, "Heure Invalide !");  // Exception si l'heure est invalide
          }
        }
        else
        {
          throw DistribException(1, "Format HHMM SVP"); // Exception si l'heure n'est pas au bon format
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
            tempInputString = "";
            break;
          
          default:
            break;
        }
      }
    }
    else {  // Si la touche choisie est un chiffre, un chiffre est ajouté à l'heure
      if (tempInputString.length() < 4){
        tempInputString += key;
      }
    }
  }

  screen.DrawKeyboard("Repas " + (String)(index+1), tempInputString, keyboardIndex, true);  // Affiche le clavier pour la suite de la saisie
}

// Gestion du menu animal
void Application::HandleMenuAnimal(InputEvent evt) {
  int itemCount = 7;  // Nombre d'items dans le menu animal
  int maxVisible = 4; // Nombre d'items maximum affichés en même temps sur l'écran

  if (evt == EVT_DOWN) {
    menuIndex++; 
    if (menuIndex > 6) {
      menuIndex = 0;
      menuScrollOffset = 0;
    }
    else if (menuIndex >= menuScrollOffset + maxVisible) {
      menuScrollOffset++; // On décale la fenêtre vers le bas pour afficher les items suivants
    }
  }
  else if (evt == EVT_UP) {
    menuIndex--; 
    if (menuIndex < 0) {
      menuIndex = itemCount - 1; // Dernier item
      menuScrollOffset = itemCount - maxVisible; 
      if (menuScrollOffset < 0) {
        menuScrollOffset = 0; // Sécurité sur le décalage de la fenêtre
      }
    }
    else if (menuIndex < menuScrollOffset) {
      menuScrollOffset--; // On décale la fenêtre vers le haut
    }
  }
  else if (evt == EVT_BACK) {
    // Retour au menu principal
    currentState = STATE_MENU_MAIN;
    menuIndex = 1;  // Préselection de l'item "Animal"
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
        screen.DrawKeyboard("Nom de l'animal", pets.getName(), 0, false); // false = Clavier Alphanumérique
        break;
      case 1: // Especes 
        currentState = STATE_SELECT_SPECIES;
        menuIndex = 0;
        screen.DrawMenu("Choix Especes", speciesItems, 5, menuIndex, 0);
        break;
      case 2: // Age
        currentState = STATE_EDIT_FIELD;
        screen.DrawKeyboard("Age", String(pets.getAge()), 0, true); // true = Clavier Numérique
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
      case 6: // Voir Fiche Informations
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

// Gestion de la selection du comportement
void Application::HandleBehaviorSelect(InputEvent evt) {
  if (evt == EVT_DOWN) {
    menuIndex++;
    if (menuIndex > 1)
      menuIndex = 0;
  }
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0)
      menuIndex = 1;
  }
  else if (evt == EVT_ENTER) {
    // Selection du comportement
    if (menuIndex == 0)
      pets.setBehavior("Calme");
    else
      pets.setBehavior("Agressif");

    // Sauvegarde en mémoire ROM
    pets.Save();

    // Retour au menu animal
    currentState = STATE_MENU_ANIMAL;
    menuIndex = 5; // Préselection de l'item "Comportement"
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, menuScrollOffset);
    return;
  }
  // Annuler choix, retour au menu animal
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

// Gestion de la selection de l'espèce
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
      menuScrollOffset++;
    }
  }
  else if (evt == EVT_UP) {
    menuIndex--; 
    if (menuIndex < 0) {
      menuIndex = itemCount - 1;
      menuScrollOffset = itemCount - maxVisible; 
      if (menuScrollOffset < 0) {
        menuScrollOffset = 0;
      }
    }
    else if (menuIndex < menuScrollOffset) {
      menuScrollOffset--;
    }
  }
  else if (evt == EVT_BACK) {
    currentState = STATE_MENU_ANIMAL;
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
    pets.Save();

    currentState = STATE_MENU_ANIMAL;
    menuIndex = 1; // On revient sur la ligne "Espece"
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, 0);
    return;
  }

  if (currentState == STATE_SELECT_SPECIES && evt != EVT_NONE) {
    screen.DrawMenu("Choix Especes", speciesItems, 5, menuIndex, menuScrollOffset);
  }
}

// Gestion du retour de la fiche information animal
void Application::HandleAnimalSummary(InputEvent evt) {
  if (evt == EVT_BACK || evt == EVT_ENTER) {  // Si le bouton retour ou entré est appuyé, on revient au menu animal
    currentState = STATE_MENU_ANIMAL;
    screen.DrawMenu("MENU ANIMAL", animalMenuItems, 7, menuIndex, 3);
  }
}

// Gestion du retour de la fiche heures des repas
void Application::HandleHourSummary(InputEvent evt) {
  if (evt == EVT_BACK || evt == EVT_ENTER) {  // Si le bouton retour ou entré est appuyé, on revient au menu horaires
    currentState = STATE_MENU_SCHEDULE;
    screen.DrawMenu("MENU HORAIRE", menuScheduleItems, 4, menuIndex, 0);
  }
}

// Gestion de la distribution manuelle
void Application::HandleManualFeed(InputEvent evt) {
  try
  {
    if (evt == EVT_ENTER) { // Si le bouton entré est appuyé, la dose de croquette est distribuée
      if (sensor.GetLevelPercent() < 10)
      {
        throw DistribException(3, "Bac à croquettes vide !");  // Exception si les croquettes sont vides au moment de la distribution
      }

      Serial.println("Distribution en cours...");
      screen.ShowMessage("Distribution en cours...");
      
      // Action Moteur (cette fonction est bloquante quelques secondes)
      distributor.ForceFeed(&motor, pets.getSpecies()->getRationCoef());
      
      // Fin de la distribution
      Serial.println("Termine !");
      screen.ShowMessage("Termine !");
      delay(1000);
      
      // On réaffiche le menu de distribution
      currentState = STATE_MANUAL_FEED;
      screen.DrawMenu("DISTRIBUTION", manualMenuItems, 1, 0, 0);
    }
    else if (evt == EVT_BACK) { // Si le bouton retour est appuyé, la dose n'est pas distribuée et on revient au menu principal
      currentState = STATE_MENU_MAIN;
      screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex, 0);
    }
  }
  catch (DistribException ex)
  {
    switch (ex.exType)
    {
      case 3:
        screen.ShowMessage(ex.exMessage);
        delay(1000);
        tempInputString = "";
        screen.Refresh();
        break;
      
      default:
        break;
    }
  }
}