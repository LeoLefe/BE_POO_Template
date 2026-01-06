/*********************************************************************
 * @file  Sensor.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Sensor
 *********************************************************************/
#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H
#include <Wire.h>
#include <VL53L0X.h>


  class Sensor
  {
    //Constantes
    #define DIST_MIN_FULL 35    // Distance en mm pour réservoir plein
    #define DIST_MAX_EMPTY 160  // Distance en mm pour réservoir vide

    // Attributs
    protected :
    VL53L0X sensor;
    
    //Constructeurs
    public :
    Sensor();

    //Déconstructeurs
    ~Sensor();

    //Méthodes
    void InitDistance();

    uint16_t ReadDistance_mm();

    // Retourne le niveau en pourcentage (0 à 100%)
    uint8_t GetNiveauPercent();
  };

#endif