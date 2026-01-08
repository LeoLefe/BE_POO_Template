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

    // 1. Dashboard
    void DrawDashboard(String timeStr, int levelPercent, String petName);

    // 2. Menu Liste
    // items: tableau de chaines, selectedIndex: quel item est en surbrillance
    void DrawMenu(String title, const char* items[], int itemCount, int selectedIndex);

    // 3. Clavier (Alpha ou Numérique)
    // keyboardType: 0 = Alpha (A-Z), 1 = Numeric (0-9)
    // currentText: ce qu'on a déjà tapé
    // selectedKey: index de la touche sélectionnée sur le clavier
    void DrawKeyboard(String title, String currentText, int selectedKey, bool isNumeric);
    
    // Helpers pour récupérer le caractère d'une touche
    char GetKeyChar(int index, bool isNumeric);
    int GetKeyCount(bool isNumeric); // Nombre total de touches

    uint16_t GetColorForLevel(int level);
    //void UpdateDashboard(String timeStr, int levelPercent);
    void ShowMessage(String msg);
    void Refresh();
};

#endif