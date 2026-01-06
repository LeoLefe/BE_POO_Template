#include "DistributionManager.h"


  //Constructeurs
  DistributionManager :: DistributionManager(){}

  DistributionManager :: ~DistributionManager(){}

  //Méthodes
  void DistributionManager :: set_heure_distribution_matin(const String& heure) {
    heure_matin = heure;
  }

  void DistributionManager :: set_heure_distribution_soir(const String& heure) {
    heure_soir = heure;
  }

  void DistributionManager :: Check_heure(Motor motor, Sensor sensor, HomeAssistantConfig HAC) {
    String heure = HAC.get_time_string();

    // Distribution du matin
    if (heure == heure_matin) {
      Serial.println("Distribution automatique du matin !");
      motor.Start_async(1, 800);
    }

    // Distribution du soir
    if (heure == heure_soir) {
      Serial.println("Distribution automatique du soir !");
      motor.Start_async(1, 800);
    }

    // Mesure du niveau de croquette
    uint16_t distance = sensor.GetNiveauPercent();
    Serial.print("Niveau croquette : ");
    Serial.print(distance);
    Serial.println(" %");

  }

  void DistributionManager :: Manuelle(int portions_manuelles, Sensor sensor){
    Serial.println(portions_manuelles);

    // Mesure du niveau de croquette
    uint16_t distance = sensor.GetNiveauPercent();
    Serial.print("Niveau croquette : ");
    Serial.print(distance);
    Serial.println(" %");
  }
