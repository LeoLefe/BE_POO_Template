/*********************************************************************
 * @file  DistributionManager.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe DistributionManager
 *********************************************************************/
#include "DistributionManager.h"

DistributionManager::DistributionManager() {
  // Initialisation par défaut pour éviter les valeurs vides
  for(int i=0; i<MAX_MEALS; i++) mealTimes[i] = "12:00:00"; 
  
  // Valeurs par défaut classiques
  mealTimes[0] = "08:00:00";
  mealTimes[1] = "19:00:00";
}

DistributionManager::~DistributionManager() {}

void DistributionManager::Init() {
  Preferences prefs;
  prefs.begin("distrib_cfg", true); // Lecture seule

  mealCount = prefs.getInt("count", 2);
  if (mealCount < 1) mealCount = 1;
  if (mealCount > MAX_MEALS) mealCount = MAX_MEALS;

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

  prefs.putInt("count", mealCount);
  
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

void DistributionManager::CheckAutoFeed(String currentTime, Motor* motor, int coeff) {
  
  int currentMinute = currentTime.substring(3, 5).toInt();  // Extraction de la minute actuelle pour le flag lastFedMinute

  if (currentMinute == lastFedMinute) return; // Si on a déjà nourri cette minute, on ne fait rien

  bool feedNow = false; // Vérification de tous les horaires
  for (int i = 0; i < mealCount; i++) {
    if (currentTime == mealTimes[i]) {
      feedNow = true;
      break;
    }
  }

  if (feedNow) {
    Serial.println("Distribution Automatique !");
    motor->Start((1.0*coeff), 800); // 1 tour
    lastFedMinute = currentMinute; // Marque la minute comme traitée
  }
}

void DistributionManager::ForceFeed(Motor* motor, int coeff) {  // Distribution manuelle
  Serial.println("Distribution Manuelle !");
  motor->Start(1.0*coeff); 
}