/*********************************************************************
 * @file  Whale.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Whale
 *********************************************************************/
#include "Whale.h"

Whale::Whale() {
  speciesName = "Baleine";
  sPClass = Mammal;
  diet = Plankton;
  lifespan = 90;
  rationCoefficient = 100;
}
Whale::~Whale() {}