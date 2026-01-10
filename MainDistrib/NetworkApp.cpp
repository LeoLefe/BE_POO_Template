/*********************************************************************
 * @file  NetworkApp.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Connexion
 *********************************************************************/
#include "NetworkApp.h"

NetworkApp::NetworkApp() : connected(false) {}
NetworkApp::~NetworkApp() {}

void NetworkApp::Init() {
  // Connexion
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false); // Désactivation du mode économie d'énergie
  WiFi.begin(ssid, password);
  Serial.print("Connexion WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 100) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnecté ! IP: " + WiFi.localIP().toString());
    
    // Config heure
    configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "pool.ntp.org", "time.nist.gov");
    connected = true;
  }
  else {
    Serial.println("\nEchec WiFi (Mode hors ligne)");
    connected = false;
  }
}

void NetworkApp::Update() {
  if (WiFi.status() != WL_CONNECTED && connected) {
    WiFi.disconnect();
    WiFi.reconnect();
  }
}

String NetworkApp::GetTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "--:--:--";
  }
  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buffer);
}