#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <PubSubClient.h>
#include "HomeAssistantConfig.h"
#include "Motor.h"
#include "Secret.h"
#include "DistributionManager.h"

// Informations WiFi
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

// Informations MQTT
const char* mqtt_server = ADRESSE_IP_BROKER;
const char* mqtt_user = USER_MQTT;
const char* mqtt_password = PASSWORD_MQTT;

// Variables globales
int portions_manuelles = 1;


WiFiClient espClient;
PubSubClient client(espClient);


// Gestion Wifi

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(ssid);

  WiFi.disconnect(true, true);  // ⚠️ force la déconnexion des anciens réseaux
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Adresse MAC ESP32 : ");
  Serial.println(WiFi.macAddress());

  unsigned long startAttemptTime = millis();
  const unsigned long wifiTimeout = 30000; // 30 secondes timeout

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
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

  client.setServer(mqtt_server, 1883);
}


// Gestion MQTT

void reconnect_mqtt() {
  while (!client.connected()) {
    
    Serial.print("Tentative de connexion à : ");
    Serial.println(mqtt_password);
    if (client.connect("ESP32_Distributeur", mqtt_user, mqtt_password)) {
      Serial.println("connecté !");
      client.subscribe("/distributeur/commande");
    } else {
      Serial.print("Echec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5s");
      delay(5000);
    }
  }
}

void mqtt_loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();
}

// Envoyer le niveau de croquette vers home assistant
void publish_niveau(uint8_t niveau) {
  char payload[8];
  sprintf(payload, "%d", niveau);
  client.publish("/distributeur/niveau", payload);
}

void set_mqtt_callback(void (*callback)(char* topic, byte* payload, unsigned int length)) {
  client.setCallback(callback);
}

// Recevoir les données depuis home assistant
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu sur le topic : ");
  Serial.println(topic);

  if (strcmp(topic, "/distributeur/nb_portions") == 0) {
    String val = String((char*)payload).substring(0, length);
    portions_manuelles = val.toInt();
    portions_manuelles = constrain(portions_manuelles, 1, 3);
    Serial.print("Nombre de portions manuelles défini à : ");
    Serial.println(portions_manuelles);
  }

  if (strcmp(topic, "/distributeur/commande") == 0) {
    Serial.println("Distribution manuelle !");
    distrib_manuelle(portions_manuelles);
  }
}


// Gestion de l'heure

void setup_time() {
  configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", "pool.ntp.org", "time.nist.gov");; // Fuseau GMT+1 (Paris)
  Serial.println("Synchronisation NTP demandée...");

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erreur de synchronisation NTP !");
    return;
  }
  Serial.println("Heure NTP synchronisée !");
}

String get_time_string() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "00:00:00"; // Erreur
  }

  char buffer[9];
  sprintf(buffer, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buffer);
}