#include "InputManager.h"

InputManager::InputManager() : lastStateManual(false), lastDebounceTime(0) {}

void InputManager::Init() {
  pinMode(PIN_BP1, INPUT);
  pinMode(PIN_BP2, INPUT);
  pinMode(PIN_BP3, INPUT);
  pinMode(PIN_BP4, INPUT);
}

InputEvent InputManager::ReadEvent() {
  if (millis() - lastDebounceTime < DEBOUNCE_DELAY) return EVT_NONE;

  InputEvent event = EVT_NONE;

  // Lecture des 4 boutons
  if (digitalRead(PIN_BP1) == HIGH) event = EVT_BACK;
  else if (digitalRead(PIN_BP2) == HIGH) event = EVT_DOWN;
  else if (digitalRead(PIN_BP3) == HIGH) event = EVT_UP;
  else if (digitalRead(PIN_BP4) == HIGH) event = EVT_ENTER;

  if (event != EVT_NONE) {
    lastDebounceTime = millis(); // Reset timer anti-rebond
    Serial.println("Bouton appuyé !");
  }

  return event;
}