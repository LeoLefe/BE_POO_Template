/*********************************************************************
 * @file  Screen.cpp
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe Screen
 *********************************************************************/
#include "Screen.h"

// --- Définition des layouts de claviers ---

// Clavier Alpha : A-Z (26) + EFFACER (index 26) + OK (index 27) -> 28 touches
const char KEYMAP_ALPHA[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ<>"; 
// '<' sera représenté par une flèche arrière, '>' par OK

// Clavier Num : 0-9 (10) + EFFACER (10) + OK (11) -> 12 touches
const char KEYMAP_NUM[] = "0123456789<>";

// Classe screen :
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

void Screen::DrawDashboard(String timeStr, int levelPercent, String petName) {
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

  // Fond noir partiel pour éviter scintillement complet
  //tft.fillRect(40, 60, 160, 120, TFT_BLACK);
  
  // Affichage Heure
  tft.setTextSize(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(timeStr, centerX, centerY - 20);

  // Affichage niveau en texte
  tft.setTextSize(2);
  String levelStr = "  " + String(levelPercent) + " %  ";
  tft.setTextColor(GetColorForLevel(levelPercent), TFT_BLACK);
  tft.drawString(levelStr, centerX, centerY + 30);

  // Affichage nom Animal
  tft.setTextSize(2);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(petName, centerX, centerY + 60);

  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY);
  tft.drawString("Appuyez pour MENU", centerX, 220);
}

void Screen::DrawMenu(String title, const char* items[], int itemCount, int selectedIndex) {
  tft.fillScreen(TFT_BLACK);

  // Titre
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(TC_DATUM); // Top Center
  tft.drawString(title, centerX, 50);

  // Liste
  int startY = 80;
  int stepY = 30;

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);

  for (int i = 0; i < itemCount; i++) {
    int yPos = startY + (i * stepY);
    
    if (i == selectedIndex) {
      // Élément sélectionné : Fond Blanc, Texte Noir
      tft.fillRoundRect(20, yPos - 12, 200, 24, 5, TFT_WHITE);
      tft.setTextColor(TFT_BLACK, TFT_WHITE);
    } else {
      // Normal : Fond Noir, Texte Blanc
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.drawString(items[i], centerX, yPos);
  }
}

void Screen::DrawKeyboard(String title, String currentText, int selectedKey, bool isNumeric) {
  tft.fillScreen(TFT_BLACK);

  // 1. Titre et Champ de saisie
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextDatum(TC_DATUM);
  tft.drawString(title, centerX, 5);

  // Boite de texte
  tft.drawRect(30, 20, 180, 30, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(currentText, centerX, 35);

  // 2. Grille de touches
  int keySize = 26; // Carré de 30px
  int gap = 4;
  int cols = 6; // 6 colonnes

  int totalKeys = isNumeric ? 12 : 28;
  const char* map = isNumeric ? KEYMAP_NUM : KEYMAP_ALPHA;

  // Calcul pour centrer le bloc entier horizontalement
  int gridWidth = cols * keySize + (cols - 1) * gap;
  int startX = (tft.width() - gridWidth) / 2; // = (240 - 176)/2 = 32
  int startY = 60;

  for (int i = 0; i < totalKeys; i++) {
    // Calcul position grille
    int col = i % cols;
    int row = i / cols;
    int x = startX + col * (keySize + gap);
    int y = startY + row * (keySize + gap);

    // --- LOGIQUE DE CENTRAGE DE LA DERNIÈRE LIGNE ---
    // Si nous sommes sur la dernière ligne (row) et qu'elle n'est pas pleine
    int keysOnLastRow = totalKeys % cols; 
    int totalRows = (totalKeys + cols - 1) / cols;
    // Si keysOnLastRow == 0, la ligne est pleine (ex: 12 touches / 6 cols = 0 reste), pas de décalage.
    // Si on est sur la dernière ligne (row == totalRows - 1) et qu'il y a un reste
    if (keysOnLastRow > 0 && row == (totalRows - 1)) {
        // Calcul du décalage pour centrer ces touches
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
    tft.setTextSize(2);
    char c = map[i];
    
    if (c == '<') tft.drawString("<-", x + keySize/2, y + keySize/2);
    else if (c == '>') tft.drawString("OK", x + keySize/2, y + keySize/2);
    else tft.drawChar(c, x + keySize/2 - 6, y + keySize/2 - 8);
  }
}

char Screen::GetKeyChar(int index, bool isNumeric) {
  if (isNumeric) return KEYMAP_NUM[index];
  return KEYMAP_ALPHA[index];
}

int Screen::GetKeyCount(bool isNumeric) {
  return isNumeric ? 12 : 28;
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