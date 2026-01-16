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
    const int ARC_RADIUS_INNER = 110; // Rayon intérieur
    const int ARC_START_ANGLE = 120;  // Angle initial
    const int ARC_END_ANGLE = 240;    // Angle final
    const int ARC_FULL_SPAN = 120;    // Angle de l'arc

    // Clavier Alphanumérique : A-Z (26) + EFFACER (index 26) + OK (index 27) => 28 touches
    // '<' sera représenté par un retour en arrière, '>' par OK
    const char KEYMAP_ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ<>"; 

    // Clavier Num : 0-9 (10) + EFFACER (10) + OK (11) => 12 touches
    // '<' sera représenté par un retour en arrière, '>' par OK
    const char KEYMAP_NUM[] = "0123456789<>";

  public:
    public:
    Screen();
    ~Screen();
    void Init();    // Ecran de démarrage
    void Refresh(); // Ecran noir

    void DrawDashboard(String timeStr, int levelPercent, String petName); // Permet d'afficher l'écran d'accueil (heure, niveau de croquette et prénom de l'animal)
    void DrawMenu(String title, const char* items[], int itemCount, int selectedIndex, int scrollOffset); // Permet d'afficher un Menu avec plusieurs items et un scroll si nécessaire
    void DrawKeyboard(String title, String currentText, int selectedKey, bool isNumeric); // Permet d'afficher un clavier numérique si isNumeric est vraie ou alphanumérique sinon
    
    // Récupération des caractères du clavier
    char GetKeyChar(int index, bool isNumeric); // Index de la touche dans le clavier
    int GetKeyCount(bool isNumeric);            // Nombre total de touches du clavier

    uint16_t GetColorForLevel(int level);   // Adaptation de la couleur en fonction du niveau de croquette
    void ShowMessage(String msg);           // Affiche un message orange d'erreur ou d'information sur un écran noir
    
    void DrawAnimalSummary(String name, String speciesName, int age, int weight, int height, String behavior);  // Permet d'afficher la fiche d'information de l'animal
    void DrawHourSummary(String H1, String H2, String H3);  // Permet d'afficher la fiche listant les heures de repas
};

#endif