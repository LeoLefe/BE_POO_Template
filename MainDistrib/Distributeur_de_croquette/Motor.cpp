#include "Motor.h"
#include <Arduino.h>

// Déclaration des broches moteur pas à pas
#define PIN_STEP 13
#define PIN_DIR  12
#define PIN_EN   16

struct MotorTaskParams {
  float nb_tours;
  int vitesse;
};

void motor_init(){
  //Init Driver moteur
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  digitalWrite(PIN_EN, HIGH); // Desactivation du moteur
  Serial.println("Moteur prêt !");
}

void motor_start(float nb_tour, int vitesse){
  // Faire avancer le moteur de nb_tour complet
  digitalWrite(PIN_EN, LOW); // LOW = moteur activé
  for (int i = 0; i < (microsteps_per_revolution*nb_tour); i++){
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(vitesse); // Ajuste pour la vitesse (plus petit = plus rapide)
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(vitesse);
  }
  delay(500);
  digitalWrite(PIN_EN, HIGH); // HIGH = moteur désactivé
}

void motor_inv(){
  // Inverser la direction du moteur
  digitalWrite(PIN_DIR, !digitalRead(PIN_DIR));

}

void motor_task(void* parameter) {
  MotorTaskParams* params = (MotorTaskParams*)parameter;
  motor_start(params->nb_tours, params->vitesse);
  delete params; // libère la mémoire allouée
  publish_niveau
  vTaskDelete(NULL); // termine la tâche
}

void motor_start_async(float nb_tours, int vitesse) {
  MotorTaskParams* params = new MotorTaskParams{nb_tours, vitesse};

  xTaskCreatePinnedToCore(
    motor_task,         // fonction
    "MotorTask",        // nom tâche
    2048,               // stack size
    params,             // paramètre
    1,                  // priorité
    NULL,               // handle (pas nécessaire ici)
    1                   // core 1 (core libre)
  );
}