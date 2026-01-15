/*********************************************************************
 * @file  Species.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Species
 *********************************************************************/
#ifndef SPECIES_H
#define SPECIES_H

#include <Arduino.h>

enum SpeciesClass {
  Mammal,
  Bird,
  Reptile,
  Amphibian,
  Insect,
  Arachnid
};

enum SpeciesDiet {
  DogKibble,
  CatKibble,
  Plankton,
  Hay,
  Cheese
};

class Species {
  protected:
    String speciesName;
    SpeciesClass sPClass;
    SpeciesDiet diet;
    int lifespan;// type de croquette différent en focntion del'âge (chaton, vieux chat...)
    int rationCoefficient;

  public:
    Species();
    virtual ~Species();

    void setSPName(String spN);
    String getSPName();

    void setSPClass(SpeciesClass spC);
    SpeciesClass getSPClass();

    void setDiet(SpeciesDiet spD);
    SpeciesDiet getDiet();
    
    void setLifespan(int spL);
    int getLifespan();
    
    virtual String getSound() = 0;
};

#endif