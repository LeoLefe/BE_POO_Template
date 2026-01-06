/*********************************************************************
 * @file  HomeAssistantConfig.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Connexion
 *********************************************************************/
#ifndef HOMEASSISTANTCONFIG_H
#define HOMEASSISTANTCONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <PubSubClient.h>
#include "Motor.h"
#include "Secret.h"


  class HomeAssistantConfig
  {
    //Constantes
    // Informations WiFi
    const char* ssid = SECRET_SSID;
    const char* password = SECRET_PASSWORD;

    public:
    //Constructeurs
    HomeAssistantConfig();

    ~HomeAssistantConfig();

    //Méthodes
    //Gestion Wifi
    void setup_wifi();

    // Gestion de l'heure
    void setup_time();
    String get_time_string();
  };


#endif // HOMEASSISTANTCONFIG_H