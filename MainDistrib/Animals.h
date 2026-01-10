/*********************************************************************
 * @file  Animals.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Animal
 *********************************************************************/
#ifndef ANIMALS_H
#define ANIMALS_H

#include <Arduino.h>
#include <Preferences.h>

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

    // Gestion de la sauvegarde
    void Load(); // Charge les données au démarrage
    void Save(); // Ecrit les données dans la mémoire flash

    void setName(String n);
    String getName();

    void setAge(int a);
    int getAge();
    
    void setHeight(int h);
    int getHeight();
    
    void setWeight(int w);
    int getWeight();

    void setBehavior(String b);
    String getBehavior();
};

#endif