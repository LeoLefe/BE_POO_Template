#include "Motor.h"
#include "Screen.h"
#include "Sensor.h"
#include "HomeAssistantConfig.h"
#include "DistributionManager.h"

#include <TFT_eSPI.h> // Librairie TFT_eSPI

//TFT_eSPI tft = TFT_eSPI(); // Crée une instance de l'écran

Motor _motor;       //Init du Driver moteur
Screen _screen;     //Init de l'écran
Sensor _sensor;     //Init du capteur de distance
HomeAssistantConfig _homeassistantconfig ;
DistributionManager _distributionmanager ;

void setup() {

  Serial.begin(115200);  //Moniteur série
  Serial.println(ESP.getSdkVersion());
  
  //Init terminée
  Serial.println("Modules initialisés !");
}

void loop() 
{

  _screen.Show_time(_homeassistantconfig);   // Affichage d'une couleur aléatoire et d'un test central

  delay(1000);

}

