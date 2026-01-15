/*********************************************************************
 * @file  Dog.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Dog
 *********************************************************************/
#ifndef DOG_H
#define DOG_H

#include <Arduino.h>
#include "Species.h"

class Dog : public Species {
  public:
    Dog();
    ~Dog();

    String getSound() override {
      return "WOUF !";
    }
};

#endif