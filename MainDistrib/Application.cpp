/*********************************************************************
 * @file  Apllication.cpp
 * @author <mettre l'adresse mail ou nom prenom>
 * @brief Fichier source de l'application
 *********************************************************************/
#include "Application.h"

Application::Application() : currentState(STATE_DASHBOARD), menuIndex(0), keyboardIndex(0), lastUIUpdate(0) {}
Application::~Application() {}

void Application::Init() {
  Serial.println("Init Application...");
  
  motor.Init();
  inputs.Init();
  screen.Init();
  network.Init(); 
  sensor.Init();
  distributor.Init();
  
  Serial.println("Application Prête !");
  screen.Refresh();
  // Ecran initial
  screen.DrawDashboard("Init...", 0, myDog.getName());
}

void Application::Run() {
  // 1. Gestion Réseau
  network.Update();
  String currentTime = network.GetTimeString();

  // 1. Lire les entrées
  InputEvent evt = inputs.ReadEvent();
  
  // 2. Machine à états
  switch (currentState) {
    case STATE_DASHBOARD:
      HandleDashboard(evt);
      
      // Update ecran de base
      if (millis() - lastUIUpdate > 1000) {
         uint8_t level = sensor.GetLevelPercent();
         screen.DrawDashboard(currentTime, level, myDog.getName()); // Mockup
         lastUIUpdate = millis();
      }
      break;

    case STATE_MENU_MAIN:
      HandleMenuMain(evt);
      break;

    case STATE_EDIT_NAME:
      HandleKeyboard(evt, false); // false = Clavier Alpha
      break;
      
    case STATE_SHOW_SCHEDULE:
      HandleScheduleView(evt);
      break;
  }

  // 3. Gestion Automatique (Horaires)
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
    if (menuIndex > 2) menuIndex = 0; // Boucle
  } 
  else if (evt == EVT_UP) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = 2;
  } 
  else if (evt == EVT_BACK) {
    currentState = STATE_DASHBOARD;
    screen.Refresh();
    screen.DrawDashboard("Wait...", 0, myDog.getName());
    return;
  }
  else if (evt == EVT_ENTER) {
    // Validation du choix
    switch (menuIndex) {
      case 0: // Horaires
        currentState = STATE_SHOW_SCHEDULE;
        screen.ShowMessage("Horaires: 08h & 19h"); // Simplifié pour l'instant
        // Ici on pourrait dessiner un écran dédié
        break;
      case 1: // Nom Animal
        currentState = STATE_EDIT_NAME;
        keyboardIndex = 0;
        tempInputString = ""; // Vide le champ
        screen.DrawKeyboard("NOM ANIMAL", tempInputString, keyboardIndex, false);
        return; // Return pour ne pas redessiner le menu
      case 2: // Retour
        screen.Refresh();
        currentState = STATE_DASHBOARD;
        break;
    }
  }

  // Si on est toujours dans le menu, on redessine pour mettre à jour la surbrillance
  if (currentState == STATE_MENU_MAIN) {
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
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
      // Sauvegarde du nom
      if (!isNumeric) myDog.setName(tempInputString);
      // else if (isNumeric) ... sauver un chiffre
      
      // Retour menu
      currentState = STATE_MENU_MAIN;
      screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
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
    currentState = STATE_MENU_MAIN;
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
    return;
  }

  // Redessiner clavier
  screen.DrawKeyboard(isNumeric ? "VALEUR" : "NOM ANIMAL", tempInputString, keyboardIndex, isNumeric);
}

void Application::HandleScheduleView(InputEvent evt) {
  // Simple écran d'attente de retour
  if (evt == EVT_BACK || evt == EVT_ENTER) {
    currentState = STATE_MENU_MAIN;
    screen.DrawMenu("MENU PRINCIPAL", mainMenuItems, 3, menuIndex);
  }
}