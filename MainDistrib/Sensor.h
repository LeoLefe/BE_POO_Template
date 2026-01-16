/*********************************************************************
 * @file  Sensor.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Sensor
 *********************************************************************/
#ifndef SENSOR_H
#define SENSOR_H

#include <Wire.h>
#include <VL53L0X.h>

class Sensor {
  private:
    VL53L0X sensor;
    bool isReady;
    
    // Calibration de la cuve (en mm)
    const uint16_t DIST_FULL = 35;   // Distance quand la cuve est pleine
    const uint16_t DIST_EMPTY = 160; // Distance quand la cuve est vide

  public:
    Sensor();
    ~Sensor();

    void Init();
    uint16_t ReadDistanceMM();  // Permet de lire le niveau de croquette manquant
    uint8_t GetLevelPercent();  // Traduis le niveau de croquette restant sous forme de pourcentage
};

#endif