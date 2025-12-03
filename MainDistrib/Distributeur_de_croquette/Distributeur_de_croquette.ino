#include "Motor.h"
#include "Screen.h"
#include "Sensor.h"
#include "HomeAssistantConfig.h"
#include "DistributionManager.h"

#include <TFT_eSPI.h> // Librairie TFT_eSPI

TFT_eSPI tft = TFT_eSPI(); // Crée une instance de l'écran


void setup() {

  Serial.begin(115200);  //Moniteur série
  Serial.println(ESP.getSdkVersion());
  
  motor_init();     //Init du Driver moteur
  screen_init();    //Init de l'écran
  distance_sensor_init(); //Init du capteur de distance
  setup_wifi();
  setup_time();
  set_mqtt_callback(mqtt_callback);

  //Init terminée
  Serial.println("Modules initialisés !");
}

void loop() {
  mqtt_loop();

  screen_show_time();           // Affichage d'une couleur aléatoire et d'un test central

  delay(1000);

}

