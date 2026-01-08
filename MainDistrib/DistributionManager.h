#ifndef DISTRIBUTIONMANAGER_H
#define DISTRIBUTIONMANAGER_H

#include <Arduino.h>
#include "Motor.h"

class DistributionManager {
  private:
    String heureMatin;
    String heureSoir;
    bool distributionFaiteCetteMinute; // Flag pour éviter répétition

  public:
    DistributionManager();
    ~DistributionManager();

    void Init();
    void CheckAutoFeed(String currentTime, Motor* motor); // Passe le moteur en paramètre
    void ForceFeed(Motor* motor);
};

#endif