/*********************************************************************
 * @file  Animals.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Animals
 *********************************************************************/
#ifndef ANIMALS_H
#define ANIMALS_H

#include <Arduino.h>
#include <Preferences.h>
#include <Species.h>

class Animals {
  private:
    String name;
    Species* speciesType;
    int age;
    int height;
    int weight;
    String color;
    String behavior;

  public:
    Animals();
    Animals(String _name, Species* _speciesType, int _age, int _height, int _weight, String _color, String _behavior);
    ~Animals();

    // Gestion de la sauvegarde
    void Load(); // Charge les données au démarrage
    void Save(); // Ecrit les données dans la mémoire flash

    void setName(String n);
    String getName();

    Species* getSpecies();

    void setAge(int a);
    int getAge();
    
    void setHeight(int h);
    int getHeight();
    
    void setWeight(int w);
    int getWeight();

    void setColor(String c);
    String getColor();

    void setBehavior(String b);
    String getBehavior();

    int getRationGrams(int ratio);
};

#endif