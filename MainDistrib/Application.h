/*********************************************************************
 * @file  Apllication.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Application
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
#include "Dog.h"
#include "Cat.h"
#include "Whale.h"
#include "Horse.h"
#include "Mouse.h"
#include "DistribException.h"

// Etats de l'application
enum AppState {
  STATE_DASHBOARD,

  // Etats pour le menu principal
  STATE_MENU_MAIN,
  STATE_MANUAL_FEED,

  // Etats pour le menu Horaires
  STATE_MENU_SCHEDULE,     // Menu liste des horaires
  STATE_EDIT_MEAL_TIME,    // Clavier pour rentrer l'heure
  STATE_SHOW_HOURS,

  // Etats pour le menu Animal
  STATE_MENU_ANIMAL,       // Le sous-menu liste
  STATE_EDIT_FIELD,        // Quand on est sur le clavier (Nom, Age, Poids...)
  STATE_SELECT_BEHAVIOR,   // Le choix Calme/Agressif
  STATE_SELECT_SPECIES,    // Le choix de l'espece
  STATE_SHOW_SUMMARY       // La fiche technique
};

class Application {
  private:
    Motor motor;
    Sensor sensor;
    Screen screen;
    NetworkApp network; 
    DistributionManager distributor;
    InputManager inputs;
    Animals pets;

    // Gestion de l'état
    AppState currentState;

    // Navigation Menu
    int menuIndex;
    int menuScrollOffset;
    const char* mainMenuItems[3] = {"Horaires", "Animal", "Distrib Manuel"};

    // Sous-menu horaires (Buffers pour l'affichage dynamique du menu)
    const char* menuScheduleItems[4] = {"Heure 1", "Heure 2", "Heure 3", "Voir horaires"};

    // Sous-menu animal
    const char* animalMenuItems[7] = {"Nom", "Espece", "Age", "Poids", "Taille", "Comportement", "Voir Fiche"};

    // Sous-menu comportement
    const char* behaviorItems[2] = {"Calme", "Agressif"};

    // Sous-menu Especes
    const char* speciesItems[5] = {"Chien", "Chat", "Baleine", "Cheval", "Souris"};
    
    // Sous-menu distribution manuelle
    const char* manualMenuItems[1] = {"Lancer la dose"};
    
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

    void HandleMenuSchedule(InputEvent evt);
    void HandleEditMealTime(InputEvent evt, bool isNumeric,  int index);
    void HandleManualFeed(InputEvent evt);

    void HandleMenuAnimal(InputEvent evt);
    void HandleBehaviorSelect(InputEvent evt);
    void HandleSpeciesSelect(InputEvent evt);
    void HandleAnimalSummary(InputEvent evt);
    void HandleHourSummary(InputEvent evt);

    void HandleKeyboard(InputEvent evt, bool isNumeric);
};

#endif