#include "DistributionManager.h"

DistributionManager::DistributionManager() {
  heureMatin = "08:00:00";
  heureSoir = "19:00:00";
  distributionFaiteCetteMinute = false;
}

DistributionManager::~DistributionManager() {}

void DistributionManager::Init() {}

void DistributionManager::CheckAutoFeed(String currentTime, Motor* motor) {
  // Logique simple pour déclencher à la seconde 00
  bool isTime = (currentTime == heureMatin || currentTime == heureSoir);
  
  if (isTime && !distributionFaiteCetteMinute) {
    Serial.println("Distribution Automatique !");
    motor->Rotate(1.0); // 1 tour
    distributionFaiteCetteMinute = true; 
  }

  // Reset du flag quand l'heure change
  if (!isTime) {
    distributionFaiteCetteMinute = false;
  }
}

void DistributionManager::ForceFeed(Motor* motor) {
  Serial.println("Distribution Manuelle !");
  motor->Rotate(0.5); 
}