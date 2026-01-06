/*********************************************************************
 * @file  Motor.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Motor
 *********************************************************************/
#ifndef MOTOR
#define MOTOR

#include <Arduino.h>

// Déclaration des broches moteur pas à pas
#define PIN_STEP 13
#define PIN_DIR  12
#define PIN_EN   16

// Nombre de micro-pas pour 1 tour complet
/*  
  200 = pas de microstep
  400 = microstep 1/2
  800 = microstep 1/4
  1600 = microstep 1/8
  3200 = microstep 1/16e
*/



  class Motor
  {
    //Constantes
    const int microsteps_per_revolution = 3200;

    // Attributs
    protected:
    float nb_tours;
    int vitesse;

    //Constructeurs
    public :
    Motor();

    //Déconstructeurs
    ~Motor();

    //Méthodes
    void Init();
    void Start(float nb_tour, int vitesse);
    void Inversion();
    void Task();
    void Start_async(float nb_tours, int vitesse);
  };

#endif //ends #ifndef MOTOR