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

class Application {
  private:
    Motor motor;
    Sensor sensor;
    Screen screen;
    NetworkApp network; 
    DistributionManager distributor;
    InputManager inputs;

    unsigned long lastUpdateUI;

  public:
    Application();
    ~Application();
    void Init();
    void Run();
};

#endif