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

void Motor::Start(float turns) {
  Enable();
  int stepsToMake = (int)(turns * MICROSTEPS_PER_REV);
  
  // Direction (par défaut horaire)
  digitalWrite(PIN_DIR, HIGH); 

  // Boucle de mouvement
  for(int i = 0; i < stepsToMake; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(500); // Ajuster vitesse ici (plus petit = plus vite)
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(500);
  }
  Disable(); // On relâche pour économiser l'énergie
}

void Motor::Start(float turns, int speed) {
  Enable();
  int stepsToMake = (int)(turns * MICROSTEPS_PER_REV);
  Serial.println(speed);
  
  // Direction (par défaut horaire)
  digitalWrite(PIN_DIR, HIGH); 

  // Boucle de mouvement
  for(int i = 0; i < stepsToMake; i++) {
    digitalWrite(PIN_STEP, HIGH);
    delayMicroseconds(speed);
    digitalWrite(PIN_STEP, LOW);
    delayMicroseconds(speed);
  }
  Disable(); // On relâche pour économiser l'énergie
}