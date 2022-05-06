#ifndef UTILS_H
#define UTILS_H

#include "../../include/Configs.h"
#include "../CheckTime/CheckTime.h"
#include "../SerialManager/SerialManager.h"
#include "../StringHelper/StringHelper.h"

//#include <MemoryUsage.h>
//#include <Arduino.h>
//#include <ESP8266WiFi.h>

void print(const String &s, const String &end = "\n");

byte analog_read(byte pin);

byte digital_read(byte pin);

void analog_write(byte pin, byte state);

void digital_write(byte pin, byte state);

#endif //UTILS_H
