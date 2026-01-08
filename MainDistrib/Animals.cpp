/*********************************************************************
 * @file  Animals.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 *********************************************************************/
#include "Animals.h"

Animals::Animals() {
  this->name = "Kida";      // Nom par défaut
  this->age = 0;
  this->height = 0;
  this->weight = 0;
  this->color = "Noir";
  this->behavior = "Calme";
}
Animals::~Animals() {}

// --- Gestion du Nom ---

void Animals::setName(String n) {
  this->name = n;
}

String Animals::getName() {
  return this->name;
}

// --- Gestion des autres attributs (pour le futur) ---

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