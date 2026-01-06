/*********************************************************************
 * @file  Sensor.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Sensor
 *********************************************************************/
#include "Sensor.h"


  //Constructeurs
  Sensor :: Sensor()
  {
    InitDistance();
  }

  //Déconstructeurs
  Sensor :: ~Sensor()
  {
    delete[] &sensor;
  }

  //Méthodes
  void Sensor :: InitDistance()
  {
    // Initialisation du bus I2C personnalisé
    Wire.begin(27, 14); // SDA sur GPIO27, SCL sur GPIO14 (comme ton schéma)
    delay(100);

    if (!sensor.init())
    {
      Serial.println("Erreur : Capteur VL53L0X non détecté !");
      while (1); // Stoppe tout si capteur absent
    }
    
    sensor.setTimeout(500);
    Serial.println("Capteur VL53L0X prêt !");
  }

  uint16_t Sensor :: ReadDistance_mm()
  {
    return sensor.readRangeSingleMillimeters();
  }

  // Retourne le niveau en pourcentage (0 à 100%)
  uint8_t Sensor :: GetNiveauPercent()
  {
    uint16_t distance = ReadDistance_mm();

    if (distance <= DIST_MIN_FULL) {
      return 100;
    }
    if (distance >= DIST_MAX_EMPTY) {
      return 0;
    }
    
    // Interpolation linéaire entre plein et vide
    float niveau = 100.0 * (DIST_MAX_EMPTY - distance) / (DIST_MAX_EMPTY - DIST_MIN_FULL);
    return (uint8_t)niveau;
  }

