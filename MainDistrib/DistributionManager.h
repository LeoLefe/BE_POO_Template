#ifndef DISTRIBUTIONMANAGER_H
#define DISTRIBUTIONMANAGER_H

#include <Arduino.h>
#include "Sensor.h"
#include "HomeAssistantConfig.h"
#include "Motor.h"


  class DistributionManager
  {
    //Attributs
    protected:
    String heure_matin = "08:00:00";
    String heure_soir = "19:00:00";

    public:

    //Constructeurs
    DistributionManager();

    ~DistributionManager();

    //Méthodes
    void set_heure_distribution_matin(const String& heure);
    void set_heure_distribution_soir(const String& heure);
    void Check_heure(Motor motor, Sensor sensor, HomeAssistantConfig HAC);
    void Manuelle(int portions_manuelles, Sensor sensor);
  };
  


#endif // DISTRIBUTIONMANAGER_H