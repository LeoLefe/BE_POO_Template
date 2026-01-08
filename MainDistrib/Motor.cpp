/*********************************************************************
 * @file  Motor.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Motor
 *********************************************************************/
#include "Motor.h"

Motor::Motor() {}
Motor::~Motor() {}

void Motor::Init() {
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  pinMode(PIN_EN, OUTPUT);
  Disable(); // Désactivé au démarrage pour ne pas chauffer
}

void Motor::Enable() {
  digitalWrite(PIN_EN, LOW); // LOW = Activé sur A4988
}

void Motor::Disable() {
  digitalWrite(PIN_EN, HIGH); // HIGH = Désactivé
}

void Motor::Rotate(float turns) {
  Enable();
  long stepsToMake = (long)(turns * TOTAL_STEPS);
  
  // Direction (par défaut horaire)
  digitalWrite(PIN_DIR, HIGH); 

  // Boucle de mouvement
  for(long i = 0; i < stepsToMake; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(500); // Ajuster vitesse ici (plus petit = plus vite)
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(500);
  }
  
  Disable(); // On relâche pour économiser l'énergie
}