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
#include "DistribException.h"

// Etats de l'application
enum AppState {
  STATE_DASHBOARD,

  // Etats pour le menu principal
  STATE_MENU_MAIN,
  STATE_SHOW_SCHEDULE,
  STATE_MANUAL_FEED,

  // Etats pour le menu Horaires
  STATE_MENU_SCHEDULE,     // Menu liste des horaires
  STATE_EDIT_MEAL_COUNT,   // Changement du nombre (1-5)
  STATE_EDIT_MEAL_TIME,    // Clavier pour rentrer l'heure

  // Etats pour le menu Animal
  STATE_MENU_ANIMAL,       // Le sous-menu liste
  STATE_EDIT_FIELD,        // Quand on est sur le clavier (Nom, Age, Poids...)
  STATE_SELECT_BEHAVIOR,   // Le choix Calme/Agressif
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
    char menuScheduleItemsBuffer[6][20]; // 6 lignes max, 20 caractères de large
    const char* menuScheduleItems[6];     // Pointeurs pour DrawMenu
    int selectedMealToEdit; // Quel repas on modifie (0, 1, 2...)

    // Sous-menu animal
    const char* animalMenuItems[6] = {"Nom", "Age", "Poids", "Taille", "Comportement", "Voir Fiche"};

    // Sous-menu comportement
    const char* behaviorItems[2] = {"Calme", "Agressif"};
    
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
    void HandleEditMealCount(InputEvent evt);
    void HandleEditMealTime(InputEvent evt, bool isNumeric);
    void HandleManualFeed(InputEvent evt);

    void HandleMenuAnimal(InputEvent evt);
    void HandleBehaviorSelect(InputEvent evt);
    void HandleAnimalSummary(InputEvent evt);

    void HandleKeyboard(InputEvent evt, bool isNumeric);
};

#endif