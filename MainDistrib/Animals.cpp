/*********************************************************************
 * @file  Animals.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Animal
 *********************************************************************/
#include "Animals.h"

Animals::Animals() {    // Caractéristiques par défaut
  /*this->name = "Kida";      
  this->age = 3;
  this->weight = 7;
  this->height = 20;
  this->color = "Noir";
  this->behavior = "Calme";*/
}
Animals::~Animals() {}

void Animals::Load() {
  Preferences prefs;
  // Ouvre l'espace de stockage "pet_data" en mode lecture seule (true)
  prefs.begin("pet_data", true); 

  // Lecture des valeurs. Le 2ème paramètre est la valeur par défaut si rien n'est trouvé.
  this->name = prefs.getString("name", "Kida");
  this->age = prefs.getInt("age", 3);
  this->weight = prefs.getInt("weight", 7);
  this->height = prefs.getInt("height", 20);
  this->behavior = prefs.getString("behav", "Calme");

  prefs.end(); // Ferme l'accès
  Serial.println("Données animal chargées depuis la mémoire !");
}

void Animals::Save() {
  Preferences prefs;
  // Ouvre l'espace "pet_data" en mode ÉCRITURE (false)
  prefs.begin("pet_data", false); 

  // Ecriture des valeurs
  prefs.putString("name", this->name);
  prefs.putInt("age", this->age);
  prefs.putInt("weight", this->weight);
  prefs.putInt("height", this->height);
  prefs.putString("behav", this->behavior);

  prefs.end();
  Serial.println("Données animal sauvegardées !");
}

void Animals::setName(String n) {
  this->name = n;
}
String Animals::getName() {
  return this->name;
}


void Animals::setAge(int a) {
  this->age = a;
}
int Animals::getAge() {
  return this->age;
}


void Animals::setHeight(int h) {
  this->height = h;
}
int Animals::getHeight() {
  return this->height;
}


void Animals::setWeight(int w) {
  this->weight = w;
}
int Animals::getWeight() {
  return this->weight;
}


void Animals::setBehavior(String b) { 
  behavior = b; 
}
String Animals::getBehavior() { 
  return behavior; 
}
