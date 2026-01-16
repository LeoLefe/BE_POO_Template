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
    public:
    Screen();
    ~Screen();
    void Init();
    void Refresh();

    void DrawDashboard(String timeStr, int levelPercent, String petName);
    void DrawMenu(String title, const char* items[], int itemCount, int selectedIndex, int scrollOffset);
    void DrawKeyboard(String title, String currentText, int selectedKey, bool isNumeric);
    
    // Helpers pour récupérer le caractère d'une touche
    char GetKeyChar(int index, bool isNumeric);
    int GetKeyCount(bool isNumeric); // Nombre total de touches

    uint16_t GetColorForLevel(int level);
    void ShowMessage(String msg);
    
    void DrawAnimalSummary(String name, String speciesName, int age, int weight, int height, String behavior);
    void DrawHourSummary(String H1, String H2, String H3);
};

#endif