/*********************************************************************
 * @file  HomeAssistantConfig.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Connexion
 *********************************************************************/
#include "HomeAssistantConfig.h"

  //Constructeurs
  HomeAssistantConfig :: HomeAssistantConfig()
  {
    //setup_wifi();
    //setup_time();
  }

  HomeAssistantConfig :: ~HomeAssistantConfig()
  {

  }

  //Méthodes
  //Gestion Wifi
  void HomeAssistantConfig :: setup_wifi() 
  {
    delay(10);
    Serial.println();
    Serial.print("Connexion à ");
    Serial.println(ssid);

    WiFi.disconnect(true, true);  // force la déconnexion des anciens réseaux
    delay(100);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Adresse MAC ESP32 : ");
    Serial.println(WiFi.macAddress());

    unsigned long startAttemptTime = millis();
    const unsigned long wifiTimeout = 30000; // 30 secondes timeout

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) 
    {
      delay(500);
      Serial.print(".");
      Serial.print("  WiFi status = ");
      Serial.println(WiFi.status());
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nEchec de connexion WiFi. Redemarrage de l'ESP32...");
      delay(5000);
      ESP.restart();
    }

    Serial.println("");
    Serial.println("WiFi connecté");
    Serial.println(WiFi.localIP());
  }


  // Gestion de l'heure
  void HomeAssistantConfig :: setup_time() 
  {
    configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "pool.ntp.org", "time.nist.gov");; // Fuseau GMT+1 (Paris)
    Serial.println("Synchronisation NTP demandée...");

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) 
    {
      Serial.println("Erreur de synchronisation NTP !");
      return;
    }
    Serial.println("Heure NTP synchronisée !");
  }

  String HomeAssistantConfig :: get_time_string() 
  {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) 
    {
      return "00:00:00"; // Erreur
    }

    char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    return String(buffer);
  }

