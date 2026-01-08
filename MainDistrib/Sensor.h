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
    const uint16_t DIST_FULL = 35;   // Distance quand plein
    const uint16_t DIST_EMPTY = 160; // Distance quand vide

  public:
    Sensor();
    ~Sensor();

    void Init();
    uint16_t ReadDistanceMM();
    uint8_t GetLevelPercent();
};

#endif