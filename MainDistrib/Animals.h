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
    ~Animals();

    // Méthodes pour le nom (utilisées dans le menu)
    void setName(String n);
    String getName();

    // Méthodes pour les autres attributs (prêtes pour les futures extensions)
    void setAge(int a);
    int getAge();
    
    void setHeight(int h);
    int getHeight();
    
    void setWeight(int w);
    int getWeight();
};

#endif