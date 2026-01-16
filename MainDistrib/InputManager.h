/*********************************************************************
 * @file  InputManager.h
 * @author <Léo Lefebvre & Estelle Coudon>
 * @brief Fichier de déclaration de la classe InputManager
 *********************************************************************/
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Arduino.h>

// Enumération des actions possibles
enum InputEvent {
  EVT_NONE,
  EVT_BACK,   // BP1
  EVT_DOWN,   // BP2
  EVT_UP,     // BP3
  EVT_ENTER   // BP4
};

class InputManager {
  private:
    const int PIN_BP1 = 32;
    const int PIN_BP2 = 33;
    const int PIN_BP3 = 25;
    const int PIN_BP4 = 26;
    bool lastStateManual;
    unsigned long lastDebounceTime;
    const unsigned long DEBOUNCE_DELAY = 300;

  public:
    InputManager();
    void Init();
    // Retourne l'événement qui vient de se produire (ou EVT_NONE)
    InputEvent ReadEvent();
};

#endif