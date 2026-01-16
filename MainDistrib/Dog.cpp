/*********************************************************************
 * @file  Dog.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Dog
 *********************************************************************/
#include "Dog.h"

Dog::Dog() {
  speciesName = "Chien";
  sPClass = Mammal;
  diet = DogKibble;
  lifespan = 11;
  rationCoefficient = 3;
}

Dog::~Dog() {}