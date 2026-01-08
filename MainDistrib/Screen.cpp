/*********************************************************************
 * @file  Screen.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Screen
 *********************************************************************/
#include "Screen.h"

Screen::Screen() {}
Screen::~Screen() {}

void Screen::Init() {
  tft.init();
  tft.setRotation(0); // Adapter selon montage (0-3)
  Refresh();
  
  centerX = tft.width() / 2;
  centerY = tft.height() / 2;
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("Demarrage...", centerX, centerY);
}

uint16_t Screen::GetColorForLevel(int level) {
  int color;
  if (level <= 20) color = TFT_RED;     
  else if (level <= 50) color = TFT_ORANGE;
  else color = TFT_GREEN;
  return color;
}

void Screen::UpdateDashboard(String timeStr, int levelPercent) {
  //Affiche du niveau par une jauge
  int angleRempli = (levelPercent * ARC_FULL_SPAN) / 100;
  int currentFillEnd = ARC_START_ANGLE + angleRempli;
  if (currentFillEnd >= 360) currentFillEnd -= 360; // Gestion du passage par 0°

  // Partie de la jauge affichant la partie vide
  if (levelPercent < 100) {
    tft.drawSmoothArc(
      centerX, centerY,
      ARC_RADIUS_OUTER, ARC_RADIUS_INNER,
      currentFillEnd, ARC_END_ANGLE,      // De l'angle calculé à 60°
      TFT_DARKGREY,                       // Couleur "vide"
      TFT_BLACK,
      true
    );
  }

  // Partie de la jauge affichant le niveau
  if (levelPercent > 0) {
    tft.drawSmoothArc(
      centerX, centerY,                   // Position centrale
      ARC_RADIUS_OUTER, ARC_RADIUS_INNER, // Rayons
      ARC_START_ANGLE, currentFillEnd,    // De 300° à l'angle calculé
      GetColorForLevel(levelPercent),     // Couleur active
      TFT_BLACK,                          // Couleur de fond (pour l'anti-aliasing)
      true                                // Bords arrondis
    );
  }
  
  
  // Affichage Heure
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(timeStr, centerX, centerY - 20);

  // Affichage niveau en texte
  tft.setTextSize(2);
  String levelStr = "  " + String(levelPercent) + " %  ";
  tft.setTextColor(GetColorForLevel(levelPercent), TFT_BLACK);
  tft.drawString(levelStr, centerX, centerY + 30);
}

void Screen::ShowMessage(String msg) {
  Refresh();
  tft.setTextColor(TFT_GOLD, TFT_TRANSPARENT);
  tft.setTextSize(2);
  tft.drawString(msg, centerX, centerY);
  delay(2000);
  Refresh();
}

void Screen::Refresh() {
  tft.fillScreen(TFT_BLACK);
}