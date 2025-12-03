#include <Arduino.h>
#include "DistributionManager.h"
#include "Sensor.h"
#include "HomeAssistantConfig.h"
#include "Motor.h"

String heure_matin = "08:00:00";
String heure_soir = "19:00:00";
String heure_reset_matin = "10:01:00";
String heure_reset_soir = "21:01:00";

extern String heure_matin;
extern String heure_soir;
extern bool distrib_matin_ok;
extern bool distrib_soir_ok;


void set_heure_distribution_matin(const String& heure) {
  heure_matin = heure;
}

void set_heure_distribution_soir(const String& heure) {
  heure_soir = heure;
}

void check_heure_distribution() {
  String heure = get_time_string();

  // Distribution du matin
  if (heure == heure_matin && !distrib_matin_ok) {
    Serial.println("Distribution automatique du matin !");
    motor_start_async(1, 800);
    distrib_matin_ok = true;
  }

  // Distribution du soir
  if (heure == heure_soir && !distrib_soir_ok) {
    Serial.println("Distribution automatique du soir !");
    motor_start_async(1, 800);
    distrib_soir_ok = true;
  }

  // Mesure du niveau de croquette
  uint16_t distance = get_niveau_percent();
  Serial.print("Niveau croquette : ");
  Serial.print(distance);
  Serial.println(" %");
  publish_niveau(distance);

  // Réinitialisation des drapeaux (1 min après chaque distribution)
  if (heure == heure_reset_matin) distrib_matin_ok = false;
  if (heure == heure_reset_soir) distrib_soir_ok = false;
}

void distrib_manuelle(int portions_manuelles){
  Serial.println(portions_manuelles);
  motor_start_async(portions_manuelles, 1500);

  // Mesure du niveau de croquette
  uint16_t distance = get_niveau_percent();
  Serial.print("Niveau croquette : ");
  Serial.print(distance);
  Serial.println(" %");
  publish_niveau(distance);

}