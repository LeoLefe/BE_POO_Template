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
  tft.setRotation(0);
  Refresh();
  
  centerX = tft.width() / 2;
  centerY = tft.height() / 2;
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setFreeFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.drawString("Demarrage...", centerX, centerY);
}

void Screen::Refresh() {
  tft.fillScreen(TFT_BLACK);
}

uint16_t Screen::GetColorForLevel(int level) {
  int color;
  if (level <= 20) color = TFT_RED;     
  else if (level <= 50) color = TFT_ORANGE;
  else color = TFT_GREEN;
  return color;
}

void Screen::DrawDashboard(String timeStr, int levelPercent, String petName) {
  // Affichage du niveau par une jauge
  int angleRempli = (levelPercent * ARC_FULL_SPAN) / 100;
  int currentFillEnd = ARC_START_ANGLE + angleRempli;
  if (currentFillEnd >= 360) 
    currentFillEnd -= 360; // Gestion du passage par 0°

  // Partie de la jauge affichant la partie vide
  if (levelPercent < 100) {
    tft.drawSmoothArc(
      centerX, centerY,
      ARC_RADIUS_OUTER, ARC_RADIUS_INNER,
      currentFillEnd, ARC_END_ANGLE,      // De l'angle calculé à 60°
      TFT_DARKGREY,                       // Couleur de la partie vide
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
      TFT_BLACK,                          // Couleur de fond
      true                                // Bords arrondis
    );
  }
  
  // Affichage de l'heure
  tft.setFreeFont(&FreeSansBold18pt7b);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(timeStr, centerX, centerY - 20);

  // Affichage du niveau en pourcentage
  tft.setFreeFont(&FreeSansBold12pt7b);
  String levelStr = "  " + String(levelPercent) + " %  ";
  tft.setTextColor(GetColorForLevel(levelPercent), TFT_BLACK);
  tft.drawString(levelStr, centerX, 35);

  // Affichage du nom de l'animal
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(petName, centerX, centerY + 60);

  tft.setTextFont(1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY);
  tft.drawString("Appuyez pour MENU", centerX, 220);
}

void Screen::DrawMenu(String title, const char* items[], int itemCount, int selectedIndex, int scrollOffset) {
  Refresh();

  // Titre
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(TC_DATUM); // Centre haut
  tft.drawString(title, centerX, 50);

  // Liste
  int startY = 80;
  int stepY = 30;
  int maxVisible = 4; // Nombre d'items maximum affichés en même temps sur l'écran

  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(MC_DATUM);

  for (int i = 0; i < maxVisible; i++) {
    int itemIndex = scrollOffset + i;
    if (itemIndex >= itemCount)
      break;  // Sécurité si on a moins de 4 items au total

    int yPos = startY + (i * stepY);
    
    if (itemIndex == selectedIndex) {
      tft.fillRoundRect(40, yPos - 12, 160, 24, 5, TFT_WHITE);  // Élément sélectionné : Fond Blanc, Texte Noir
      tft.setTextColor(TFT_BLACK, TFT_WHITE);
    }
    else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK); // Élément non sélectionné : Fond Noir, Texte Blanc
    }
    tft.drawString(items[itemIndex], centerX, yPos);
  }

  // Gestion des flèches de scroll
  int arrowX = 25; // Position en X des flèches
  
  // Flèche HAUT
  if (scrollOffset > 0) {
    tft.fillTriangle(arrowX, 90, arrowX - 5, 100, arrowX + 5, 100, TFT_DARKGREY); // Triangle pointant vers le haut
  }

  // Flèche BAS
  if ((scrollOffset + maxVisible) < itemCount) {
    tft.fillTriangle(arrowX, 160, arrowX - 5, 150, arrowX + 5, 150, TFT_DARKGREY); // Triangle pointant vers le bas 
  }
}

void Screen::DrawKeyboard(String title, String currentText, int selectedKey, bool isNumeric) {
  tft.fillScreen(TFT_BLACK);

  // Titre et Champ de saisie
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(TC_DATUM);
  tft.drawString(title, centerX, 25);

  // Boite de texte
  tft.drawRoundRect(30, 43, 180, 30, 4, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(currentText, centerX, 58);

  // Grille de touches
  int keySize = 26; // Carré de 30px
  int gap = 4;
  int cols = 6; // 6 colonnes

  int totalKeys = isNumeric ? 12 : 28; // Si isNumeric == true => totalkeys = 12, sinon => totalkeys = 28
  const char* map = isNumeric ? KEYMAP_NUM : KEYMAP_ALPHA;  // De même pour map

  // Calcul pour centrer le bloc entier horizontalement
  int gridWidth = cols * keySize + (cols - 1) * gap;
  int startX = (tft.width() - gridWidth) / 2;
  int startY = 80;

  for (int i = 0; i < totalKeys; i++) {
    // Calcul position grille
    int col = i % cols;
    int row = i / cols;
    int x = startX + col * (keySize + gap);
    int y = startY + row * (keySize + gap);

    // Gestion de la dernière ligne
    int keysOnLastRow = totalKeys % cols; // Si keysOnLastRow == 0, la ligne est pleine (ex: 12 touches / 6 cols = 0 reste), pas de décalage.
    int totalRows = (totalKeys + cols - 1) / cols;
    
    if (keysOnLastRow > 0 && row == (totalRows - 1)) {
        // Calcul du décalage pour centrer les touches de la dernière ligne
        int widthOfLastRow = keysOnLastRow * keySize + (keysOnLastRow - 1) * gap;
        int emptySpace = gridWidth - widthOfLastRow;
        x += emptySpace / 2; // On décale vers la droite de la moitié de l'espace vide
    }

    // Dessin touche
    bool isSelected = (i == selectedKey);
    uint16_t bgColor = isSelected ? TFT_WHITE : TFT_DARKGREY;
    uint16_t txtColor = isSelected ? TFT_BLACK : TFT_WHITE;

    tft.fillRoundRect(x, y, keySize, keySize, 4, bgColor);
    
    // Dessin lettre ou symbole
    tft.setTextColor(txtColor, bgColor);
    tft.setFreeFont(&FreeSansBold9pt7b);
    char c = map[i];
    
    if (c == '<')
      tft.drawString("<-", x + keySize/2, y + keySize/2-2);
    else if (c == '>')
      tft.drawString("OK", x + keySize/2, y + keySize/2-2);
    else
      tft.drawChar(c, x + keySize/2 - 6, y + keySize/2+6);
  }
}

char Screen::GetKeyChar(int index, bool isNumeric) {
  if (isNumeric) {
    return KEYMAP_NUM[index];
  }
  return KEYMAP_ALPHA[index];
}

int Screen::GetKeyCount(bool isNumeric) {
  return isNumeric ? 12 : 28;
}

void Screen::ShowMessage(String msg) {
  Refresh();
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.drawString(msg, centerX, centerY);
}

void Screen::DrawAnimalSummary(String name, String speciesName, int age, int weight, int height, String behavior) {
  Refresh();

  // Titre
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("FICHE ANIMAL", centerX, 30);

  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TL_DATUM); // Bord gauche pour aligner le texte
  
  int startX = 20;
  int startY = 60;
  int gap = 28;

  // Liste des informations
  tft.drawString("Nom : " + name + " -> " + speciesName, startX, startY);
  tft.drawString("Age : " + String(age) + " ans", startX, startY + gap);
  tft.drawString("Poids : " + String(weight) + " kg", startX, startY + gap*2);
  tft.drawString("Taille : " + String(height) + " cm", startX, startY + gap*3);
  tft.drawString("Caractere : " + behavior, startX, startY + gap*4);
}


void Screen::DrawHourSummary(String H1, String H2, String H3) {
  Refresh();

  // Titre
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Fiche Horaires", centerX, 30);

  tft.setFreeFont(&FreeSansBold9pt7b);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  
  int startX = 20;
  int startY = 60;
  int gap = 28;

  // Liste des heures de repas
  tft.drawString("Heure 1 : " + H1, startX, startY);
  tft.drawString("Heure 2 : " + H2, startX, startY + gap);
  tft.drawString("Heure 3 : " + H3, startX, startY + gap*2);
  
}
