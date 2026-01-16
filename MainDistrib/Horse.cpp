/*********************************************************************
 * @file  Horse.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Horse
 *********************************************************************/
#include "Horse.h"

Horse::Horse() {
  speciesName = "Cheval";
  sPClass = Mammal;
  diet = Hay;
  lifespan = 25;
  rationCoefficient = 4;
}
Horse::~Horse() {}