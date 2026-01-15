/*********************************************************************
 * @file  Mouse.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Mouse
 *********************************************************************/
#include "Mouse.h"

Mouse::Mouse() {
  speciesName = "Souris";
  sPClass = Mammal;
  diet = Cheese;
  lifespan = 2;
  rationCoefficient = 3;
}
Mouse::~Mouse() {}