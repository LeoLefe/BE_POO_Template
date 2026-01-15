/*********************************************************************
 * @file  Horse.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Horse
 *********************************************************************/
#ifndef HORSE_H
#define HORSE_H

#include <Arduino.h>
#include "Species.h"

class Horse : public Species {
  public :
    Horse();
    ~Horse();

    String getSound() override {
      return "HIIIII-HOUUU-HOUUU !";
    }
};

#endif