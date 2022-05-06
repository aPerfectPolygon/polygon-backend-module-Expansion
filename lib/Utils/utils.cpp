#include "utils.h"

void print(const String &s, const String &end) {
	if (DEBUG) Serial.print(s + end);
}

byte analog_read(byte pin) {
	return map(analogRead(pin), 0, 1023, 0, 100);
}

byte digital_read(byte pin) {
	return digitalRead(pin) * 100;
}

void analog_write(byte pin, byte state) {
	analogWrite(pin, int(map(state, 0, 100, 0, 1023)));
}

void digital_write(byte pin, byte state) {
	digitalWrite(pin, state);
}
