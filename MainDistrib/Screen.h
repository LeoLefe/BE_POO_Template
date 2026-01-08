/*********************************************************************
 * @file  Screen.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Screen
 *********************************************************************/
#ifndef SCREEN_H
#define SCREEN_H

#include <TFT_eSPI.h> 
#include <SPI.h>



class Screen {
  private:
    TFT_eSPI tft = TFT_eSPI();
    int centerX, centerY;

    // Configuration de l'arc de cercle
    const int ARC_RADIUS_OUTER = 118; // Rayon extérieur
    const int ARC_RADIUS_INNER = 110;
    const int ARC_START_ANGLE = 120;
    const int ARC_END_ANGLE = 240;
    const int ARC_FULL_SPAN = 120;   //angle de l'arc

  public:
    Screen();
    ~Screen();

    void Init();
    uint16_t GetColorForLevel(int level);
    void UpdateDashboard(String timeStr, int levelPercent);
    void ShowMessage(String msg);
    void Refresh();
};

#endif