/*********************************************************************
 * @file  Apllication.h
 * @author <mettre l'adresse mail ou nom prenom>
 * @brief Fichier header de l'application
 *********************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Motor.h"
#include "Sensor.h"
#include "Screen.h"
#include "NetworkApp.h"
#include "DistributionManager.h"
#include "InputManager.h"
#include "Animals.h"

// Etats de l'application
enum AppState {
  STATE_DASHBOARD,
  STATE_MENU_MAIN,
  STATE_SHOW_SCHEDULE,
  STATE_EDIT_NAME
};

class Application {
  private:
    Motor motor;
    Sensor sensor;
    Screen screen;
    NetworkApp network; 
    DistributionManager distributor;
    InputManager inputs;
    Animals myDog;

    // Gestion de l'état
    AppState currentState;

    // Navigation Menu
    int menuIndex;
    const char* mainMenuItems[3] = {"Horaires", "Nom Animal", "Retour"};
    
    // Navigation Clavier
    String tempInputString; // Ce qu'on tape
    int keyboardIndex;

    unsigned long lastUIUpdate;

  public:
    Application();
    ~Application();
    void Init();
    void Run();
    // Fonctions de logique interne
    void HandleDashboard(InputEvent evt);
    void HandleMenuMain(InputEvent evt);
    void HandleKeyboard(InputEvent evt, bool isNumeric);
    void HandleScheduleView(InputEvent evt);
};

#endif