/*********************************************************************
 * @file  Animals.h
 * @author <Léo Lefebvre & Estelle Coudon>
 *********************************************************************/
#ifndef ANIMALS_H
#define ANIMALS_H

#include <Arduino.h>

class Animals {
  private:
    String name;
    int age;
    int height;
    int weight;
    String color;
    String behavior;

  public:
    Animals();
};

#endif