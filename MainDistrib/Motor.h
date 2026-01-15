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
    const int PIN_DIR = 12;
    const int PIN_STEP = 13;
    const int PIN_EN = 16;


    // Nombre de micro-pas pour 1 tour complet
    /*  
      200 = pas de microstep
      400 = microstep 1/2
      800 = microstep 1/4
      1600 = microstep 1/8
      3200 = microstep 1/16e
    */
    const int MICROSTEPS_PER_REV = 3200;

  public:
    Motor();
    ~Motor();

    void Init();
    void Enable();
    void Disable();

    // Fait tourner le moteur (fonction bloquante pour l'instant pour assurer la dose)
    void Start(float turns);
    void Start(float turns, int speed); // Réglable en vitesse
};

#endif