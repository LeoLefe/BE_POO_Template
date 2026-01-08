/*********************************************************************
 * @file  Apllication.cpp
 * @author <mettre l'adresse mail ou nom prenom>
 * @brief Fichier source de l'application
 *********************************************************************/
#include "Application.h"

Application::Application() : lastUpdateUI(0) {}
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
}

void Application::Run() {
  // 1. Gestion Réseau
  network.Update();
  String currentTime = network.GetTimeString();

  // 2. Gestion Inputs (Bouton Manuel)
  if (inputs.IsManualFeedRequested()) {
    screen.ShowMessage("Distribution...");
    distributor.ForceFeed(&motor);
  }

  // 2. Gestion Inputs (Bouton Manuel)
  if (inputs.Alphabet()) {
    screen.ShowMessage("azertyuiopqsdfghj");
  }

  // 3. Gestion Automatique (Horaires)
  distributor.CheckAutoFeed(currentTime, &motor);

  // 4. Mise à jour Interface
  if (millis() - lastUpdateUI > 500) {
    uint8_t level = sensor.GetLevelPercent();
    screen.UpdateDashboard(currentTime, level);
    lastUpdateUI = millis();
  }
}