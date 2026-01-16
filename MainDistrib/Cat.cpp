/*********************************************************************
 * @file  Cat.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Cat
 *********************************************************************/
#include "Cat.h"

Cat::Cat() {
  speciesName = "Chat";
  sPClass = Mammal;
  diet = CatKibble;
  lifespan = 18;
  rationCoefficient = 2;
}
Cat::~Cat() {}