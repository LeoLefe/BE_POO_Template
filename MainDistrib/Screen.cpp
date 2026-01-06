/*********************************************************************
 * @file  Screen.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Screen
 *********************************************************************/
#include "Screen.h"


  //Constructeurs
  Screen :: Screen()
  {
    //Init();
  }

  //Déconstructeurs
  Screen :: ~Screen(){}

  //Méthodes
  void Screen::Init()
  {
    //Init SPI
    tft.init();
    tft.setRotation(2);
    tft.fillScreen(TFT_BLACK); // Affiche du texte au milieu
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Centre le texte
    centerX = tft.width() / 2;
    centerY = tft.height() / 2;
    tft.drawString("Ecran OK !", centerX, centerY);
    Serial.println("Ecran prêt !");
  }

  void Screen::Test()
  {
    // Efface l'écran avec une couleur aléatoire
    uint16_t randomColor = tft.color565(random(0, 256), random(0, 256), random(0, 256));
    tft.fillScreen(randomColor);

    // Dessine un cercle blanc au centre
    tft.fillCircle(centerX, centerY, 60, TFT_WHITE);

    // Affiche du texte au milieu
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextDatum(MC_DATUM); // Centre le texte
    tft.drawString("Ecran OK !", centerX, centerY);
  }

  void Screen::Show_time(HomeAssistantConfig HAC)
  {
    String heure = HAC.get_time_string();

    //tft.fillScreen(TFT_BLACK); // Nettoyer l'écran
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(4);
    tft.setTextDatum(MC_DATUM); // MC_DATUM : Milieu Centre
    tft.drawString(heure, tft.width() / 2, tft.height() / 2); // Affiche l'heure au centre
  }
