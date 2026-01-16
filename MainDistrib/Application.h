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
  //Etat pour l'écran d'accueil
  STATE_DASHBOARD,

  // Etats pour le menu principal
  STATE_MENU_MAIN,
  STATE_MANUAL_FEED,

  // Etats pour le menu Horaires
  STATE_MENU_SCHEDULE,     // Menu liste des horaires
  STATE_EDIT_MEAL_TIME,    // Clavier pour rentrer l'heure
  STATE_SHOW_HOURS,        // La fiche d'heures des repas

  // Etats pour le menu Animal
  STATE_MENU_ANIMAL,       // Le sous-menu avec la liste des items
  STATE_EDIT_FIELD,        // Clavier pour modifier les items (Nom, Age, Poids...)
  STATE_SELECT_BEHAVIOR,   // Le choix du comportement
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

    // Gestion de l'état de l'application
    AppState currentState;

    // Navigation dans un menu
    int menuIndex;
    int menuScrollOffset;

    // Menu principal
    const char* mainMenuItems[3] = {"Horaires", "Animal", "Distrib Manuel"};

    // Sous-menu horaires
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
    String tempInputString; // Ce qu'on entre avec les touches
    int keyboardIndex;      // Curseur clavier

    unsigned long lastUIUpdate;

  public:
    Application();
    ~Application();

    void Init();  // Initialisation de l'application et de l'écran d'accueil
    void Run();   // Boucle de gestion de l'affichage des menus en fonction de l'état de l'application

    // Fonctions de logique interne des menus
    void HandleDashboard(InputEvent evt);
    void HandleKeyboard(InputEvent evt, bool isNumeric);

    void HandleMenuMain(InputEvent evt);

    void HandleMenuSchedule(InputEvent evt);
    void HandleEditMealTime(InputEvent evt, bool isNumeric,  int index);
    void HandleManualFeed(InputEvent evt);

    void HandleMenuAnimal(InputEvent evt);
    void HandleBehaviorSelect(InputEvent evt);
    void HandleSpeciesSelect(InputEvent evt);
    void HandleAnimalSummary(InputEvent evt);
    void HandleHourSummary(InputEvent evt);

};

#endif