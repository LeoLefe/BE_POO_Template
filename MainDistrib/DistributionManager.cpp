/*********************************************************************
 * @file  DistributionManager.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe DistributionManager
 *********************************************************************/
#include "DistributionManager.h"

DistributionManager::DistributionManager() {
  // Valeurs par défaut classiques
  mealTimes[0] = "08:00:00";
  mealTimes[1] = "13:30:00";
  mealTimes[2] = "19:00:00";
}

DistributionManager::~DistributionManager() {}

void DistributionManager::Init() {
  screen.Init();
  Preferences prefs;
  prefs.begin("distrib_cfg", true); // Lecture seule

  for (int i = 0; i < MAX_MEALS; i++) {
    String key = "time_" + String(i);
    // On charge, sinon valeur par défaut
    mealTimes[i] = prefs.getString(key.c_str(), "12:00:00");
  }
  prefs.end();
  Serial.println("Horaires chargés !");
}

void DistributionManager::Save() {
  Preferences prefs;
  prefs.begin("distrib_cfg", false); // Ecriture
  
  for (int i = 0; i < MAX_MEALS; i++) {
    String key = "time_" + String(i);
    prefs.putString(key.c_str(), mealTimes[i]);
  }
  
  prefs.end();
  Serial.println("Horaires sauvegardés !");
}

String DistributionManager::getMealTime(int index) {
  if (index < 0 || index >= MAX_MEALS)
    return "00:00:00";
  
  return mealTimes[index].substring(0, 5); // On retourne juste HH:MM pour l'affichage (sans les secondes)
}
void DistributionManager::setMealTime(int index, String timeStr) {
  if (index < 0 || index >= MAX_MEALS)
    return;
  
  if (timeStr.length() == 5)
    timeStr += ":00";  // On s'assure que le format est HH:MM:00 pour la comparaison
  mealTimes[index] = timeStr;
}

void DistributionManager::CheckAutoFeed(String currentTime, Motor* motor, int coeff, int level) {
  int currentMinute = currentTime.substring(3, 5).toInt();  // Extraction de la minute actuelle pour le flag lastFedMinute

  if (currentMinute == lastFedMinute) return; // Si on a déjà nourri cette minute, on ne fait rien

  bool feedNow = false; // Vérification de tous les horaires
  for (int i = 0; i < MAX_MEALS; i++) {
    if (currentTime == mealTimes[i]) {
      
      if (level < 10)
      {
        throw DistribException(3, "Bac à croquettes vide !");  // Exception si les croquettes sont vides au moment de la distribution
      }

      feedNow = true;
      break;
    }
  }

  if (feedNow) {
    Serial.println("Distribution Automatique !");
    screen.ShowMessage("Distribution en cours...");
    motor->Start((1.0*coeff), 800); // 1 tour
    lastFedMinute = currentMinute; // Marque la minute comme traitée
    screen.Refresh();
  }
}

void DistributionManager::ForceFeed(Motor* motor, int coeff) {  // Distribution manuelle
  Serial.println("Distribution Manuelle !");
  motor->Start(1.0*coeff); 
}