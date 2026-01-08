/*********************************************************************
 * @file  NetworkApp.h
 * @author <Léo Lefebvre & Estelle Coudon>
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

    void Init();     
    void Update();   
    String GetTimeString();
};

#endif