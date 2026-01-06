/*********************************************************************
 * @file  Screen.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Screen
 *********************************************************************/
#ifndef SCREEN
#define SCREEN

#include "HomeAssistantConfig.h"
#include <time.h>
#include <TFT_eSPI.h> // Librairie TFT_eSPI
#include <SPI.h>


class Screen
{
  // Attributs
  protected :
  TFT_eSPI tft; //extern

  int centerX;
  int centerY;

  //Constructeurs
  public :
  Screen();

  //Déconstructeurs
  ~Screen();

  //Méthodes
  void Init();
  void Test();
  void Show_time(HomeAssistantConfig HAC);
};  


#endif //ends #ifndef SCREEN