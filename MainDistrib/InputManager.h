#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Arduino.h>

class InputManager {
  private:
    const int PIN_BP1 = 32;
    const int PIN_BP2 = 33;
    const int PIN_BP3 = 25;
    const int PIN_BP4 = 26;
    bool lastStateManual;
    unsigned long lastDebounceTime;
    const unsigned long DEBOUNCE_DELAY = 50;

  public:
    InputManager();
    void Init();
    
    bool IsManualFeedRequested();
    bool Alphabet();
};

#endif