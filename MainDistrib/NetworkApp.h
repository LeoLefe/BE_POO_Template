/*********************************************************************
 * @file  NetworkApp.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Connexion
 *********************************************************************/
#ifndef NETWORKAPP_H
#define NETWORKAPP_H

#include <WiFi.h>
#include <time.h>
#include "Secret.h"

class NetworkApp {
  private:
    const char* ssid = SECRET_SSID;
    const char* password = SECRET_PASSWORD;
    bool connected;

  public:
    NetworkApp();
    ~NetworkApp();

    void Init();    // Etablissement de la connexion WIFI
    void Update();  // Actualisation de la connexion WIFI en cas de coupure
    String GetTimeString(); // Permet de récupérer l'heure locale convertie en string
};

#endif