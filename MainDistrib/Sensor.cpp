/*********************************************************************
 * @file  Sensor.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Sensor
 *********************************************************************/
#include "Sensor.h"

Sensor::Sensor() : isReady(false) {}
Sensor::~Sensor() {}

void Sensor::Init() {
  Wire.begin(27, 14); // SDA, SCL
  delay(100);

  sensor.setTimeout(500);
  if (!sensor.init()) {
    Serial.println("ERREUR: Capteur VL53L0X non détecté !");
    isReady = false;
  } else {
    sensor.startContinuous();
    isReady = true;
    Serial.println("Capteur VL53L0X prêt.");
  }
}

uint16_t Sensor::ReadDistanceMM() {
  if (!isReady) return 9999; // Valeur d'erreur
  return sensor.readRangeContinuousMillimeters();
}

uint8_t Sensor::GetLevelPercent() {
  if (!isReady) return 0;

  uint16_t dist = ReadDistanceMM();
  
  // Bornage des valeurs
  if (dist > 8000) return 0; // Erreur de lecture (out of range)
  if (dist >= DIST_EMPTY) return 0;
  if (dist <= DIST_FULL) return 100;

  // Produit en croix inversé (plus la distance est grande, moins il y a de croquettes)
  long level = map(dist, DIST_EMPTY, DIST_FULL, 0, 100);
  return (uint8_t)level;
}