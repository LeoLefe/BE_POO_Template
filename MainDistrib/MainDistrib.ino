#include "Application.h"

Application app;

void setup() {
  Serial.begin(115200);
  app.Init();
}

void loop() {
  app.Run();
}