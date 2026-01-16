/*********************************************************************
 * @file  Species.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Species
 *********************************************************************/
#include "Species.h"

Species::Species() {}
Species::~Species() {}

void Species::setSPName(String n) {
  this->speciesName = n;
}
String Species::getSPName() {
  return this->speciesName;
}

void Species::setSPClass(SpeciesClass spC) {
  this->sPClass = spC;
}
SpeciesClass Species::getSPClass() {
  return this->sPClass;
}

void Species::setDiet(SpeciesDiet spD) {
  this->diet = spD;
}
SpeciesDiet Species::getDiet() {
  return this->diet;
}

void Species::setLifespan(int spL) {
  this->lifespan = spL;
}
int Species::getLifespan() {
  return this->lifespan;
}

void Species::setRationCoef(int spRC) {
  this->rationCoefficient = spRC;
}
int Species::getRationCoef() {
  return this->rationCoefficient;
}