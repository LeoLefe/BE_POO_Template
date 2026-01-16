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
    String speciesName;     // Nom de l'espèce
    SpeciesClass sPClass;   // Classe de l'espèce
    SpeciesDiet diet;       // Régime de l'espèce
    int lifespan;           // Durée de vie de l'espèce
    int rationCoefficient;  // Facteur correspondant à la quantité de nourriture

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

    void setRationCoef(int spRC);
    int getRationCoef();
    
    virtual String getSound() = 0;  // Méthode abstraite permettant de renseigner le cri de chaque espèce
};

#endif