/*********************************************************************
 * @file  DistributionManager.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe DistributionManager
 *********************************************************************/
#ifndef DISTRIBUTIONMANAGER_H
#define DISTRIBUTIONMANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include "Motor.h"

class DistributionManager {
  private:
    static const int MAX_MEALS = 5; // Maximum 10 repas par jour
    String mealTimes[MAX_MEALS];    // Tableau des heures "HH:MM:00"
    int mealCount;                  // Nombre de repas actifs (1 à 5)
    int lastFedMinute = -1;         // Flag pour éviter répétition

  public:
    DistributionManager();
    ~DistributionManager();

    void Init();  // Charge la config
    void Save();  // Sauvegarde la config

    int getMealCount();
    void setMealCount(int n);
    
    String getMealTime(int index);
    void setMealTime(int index, String timeStr); // format "HH:MM"

    void CheckAutoFeed(String currentTime, Motor* motor, int coeff); // Passe le moteur en paramètre
    void ForceFeed(Motor* motor, int coeff);
};

#endif