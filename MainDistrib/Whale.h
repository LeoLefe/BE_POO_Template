/*********************************************************************
 * @file  Whale.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Whale
 *********************************************************************/
#ifndef WHALE_H
#define WHALE_H

#include <Arduino.h>
#include "Species.h"

class Whale : public Species {
  public :
    Whale();
    ~Whale();

    String getSound() override {
      return "PFFFF-WHOOOOSH !";
    }
};

#endif