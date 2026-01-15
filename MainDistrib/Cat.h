/*********************************************************************
 * @file  Cat.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Cat
 *********************************************************************/
#ifndef CAT_H
#define CAT_H

#include <Arduino.h>
#include "Species.h"

class Cat : public Species {
  public :
    Cat();
    ~Cat();

    String getSound() override {
      return "MIAOU !";
    }
};

#endif