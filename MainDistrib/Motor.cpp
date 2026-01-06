/*********************************************************************
 * @file  Motor.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Motor
 *********************************************************************/
#include "Motor.h"


  //Constructeurs
  Motor :: Motor()
  {
    Init();
  }

  //Déconstructeurs
  Motor :: ~Motor(){}

  //Méthodes
  void Motor :: Init()
  {
    //Init Driver moteur
    pinMode(PIN_STEP, OUTPUT);
    pinMode(PIN_DIR, OUTPUT);
    pinMode(PIN_EN, OUTPUT);
    digitalWrite(PIN_EN, HIGH); // Désactivation du moteur
    Serial.println("Moteur prêt !");
  }

  void Motor :: Start(float nb_tour, int vitesse)
  {
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

  void Motor :: Inversion()
  {
    // Inverser la direction du moteur
    digitalWrite(PIN_DIR, !digitalRead(PIN_DIR));

  }

  void Motor :: Task()
  {
    Start(nb_tours, vitesse);
    vTaskDelete(NULL); // termine la tâche
  }

  void Motor :: Start_async(float nb_tours, int vitesse)
  {
    
  }
