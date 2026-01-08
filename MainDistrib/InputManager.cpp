#include "InputManager.h"

InputManager::InputManager() : lastStateManual(false), lastDebounceTime(0) {}

void InputManager::Init() {
  pinMode(PIN_BP1, INPUT);
  pinMode(PIN_BP2, INPUT);
  pinMode(PIN_BP3, INPUT);
  pinMode(PIN_BP4, INPUT);
}

bool InputManager::IsManualFeedRequested() {
  bool currentState = digitalRead(PIN_BP1);
  bool isPressed = false;

  // Détection de front montant
  if (currentState == HIGH && lastStateManual == LOW) {
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
      isPressed = true;
      lastDebounceTime = millis();
    }
  }
  lastStateManual = currentState;
  return isPressed;
}

bool InputManager::Alphabet() {
  bool currentState = digitalRead(PIN_BP4);
  bool isPressed = false;

  if (currentState == HIGH && lastStateManual == LOW) {
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
      isPressed = true;
      lastDebounceTime = millis();
    }
  }
  lastStateManual = currentState;
  return isPressed;
}