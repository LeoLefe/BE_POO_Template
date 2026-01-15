/*********************************************************************
 * @file  Mouse.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Mouse
 *********************************************************************/
#ifndef MOUSE_H
#define MOUSE_H

#include <Arduino.h>
#include "Species.h"

class Mouse : public Species {
  public :
    Mouse();
    ~Mouse();

    String getSound() override {
      return "COUIC-COUIC !";
    }
};

#endif