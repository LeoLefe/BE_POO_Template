/*********************************************************************
 * @file  Motor.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Motor
 *********************************************************************/
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
  private:
    const int PIN_STEP = 13;
    const int PIN_DIR  = 12;
    const int PIN_EN   = 16;
    
    // Configuration mécanique
    const int STEPS_PER_REV = 200; 
    const int MICROSTEPS = 16; // Si configuré en 1/16 sur le driver
    const int TOTAL_STEPS = STEPS_PER_REV * MICROSTEPS;

  public:
    Motor();
    ~Motor();

    void Init();
    void Enable();
    void Disable();
    
    // Fait tourner le moteur (fonction bloquante pour l'instant pour assurer la dose)
    void Rotate(float turns);
};

#endif