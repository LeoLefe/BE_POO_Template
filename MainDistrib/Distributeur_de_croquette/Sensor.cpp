#include <Wire.h>
#include <VL53L0X.h>
#include "Sensor.h"

#define DIST_MIN_FULL 35    // Distance en mm pour réservoir plein
#define DIST_MAX_EMPTY 160  // Distance en mm pour réservoir vide

// Création de l'objet capteur
VL53L0X sensor;

void distance_sensor_init() {
  // Initialisation du bus I2C personnalisé
  Wire.begin(27, 14); // SDA sur GPIO27, SCL sur GPIO14 (comme ton schéma)
  delay(100);

  if (!sensor.init()) {
    Serial.println("Erreur : Capteur VL53L0X non détecté !");
    while (1); // Stoppe tout si capteur absent
  }
  
  sensor.setTimeout(500);
  Serial.println("Capteur VL53L0X prêt !");
}

uint16_t distance_sensor_read_mm() {
  return sensor.readRangeSingleMillimeters();
}

// Retourne le niveau en pourcentage (0 à 100%)
uint8_t get_niveau_percent() {
  uint16_t distance = distance_sensor_read_mm();

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
