#ifndef HOMEASSISTANTCONFIG_H
#define HOMEASSISTANTCONFIG_H

#include <Arduino.h>

void setup_wifi();

void reconnect_mqtt();
void mqtt_loop();
void publish_niveau(uint8_t niveau);
void set_mqtt_callback(void (*callback)(char* topic, byte* payload, unsigned int length));
void mqtt_callback(char* topic, byte* payload, unsigned int length);

void setup_time();
String get_time_string();

#endif // HOMEASSISTANTCONFIG_H