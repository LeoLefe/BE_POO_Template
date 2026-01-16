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

    // Nombre de micro-pas pour 1 tour complet de moteur
    /*  
      200 = pas de microstep
      400 = microstep 1/2
      800 = microstep 1/4
      1600 = microstep 1/8
      3200 = microstep 1/16e
    */

    // Nombre de micro-pas à multiplier par 3 par rapport à l'engrenage de la vis sans fin
    const int MICROSTEPS_PER_REV = 3200*3;

  public:
    Motor();
    ~Motor();

    void Init();    // Configuration matérielle du moteur
    void Enable();  // Mise sous couple du moteur
    void Disable(); // Mise hors couple du moteur

    // Fait tourner le moteur pour distribuer les croquettes
    void Start(float turns);
    void Start(float turns, int speed); // Surchage de la focntion pour utiliser une vitesse réglable
};

#endif