#include "SerialManager.h"

void SerialManager::begin() {
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	Serial.begin(baud_rate);
}

void SerialManager::print(const String &t) {
	if (DEBUG) Serial.print(t + end);
}

void SerialManager::send(const String &t) {
	Serial.print(MID_MV + t + end);
}

void SerialManager::log(byte _tag, uint16_t _loc, uint16_t _comment, byte _pin, byte _state) {
	if (!mask_any_false) {
		digitalWrite(13, HIGH);
		print(F("SerialManager.log OverFlow"));
		return;
	}
	if (_tag < filter_tags_above) {
		return;
	}

	byte free_index = buffer_len;
	mask_any_false = false;

	// get free index from mask
	for (byte i = 0; i < buffer_len; i++) {
		if (!mask[i]) {
			if (free_index != buffer_len) {  // previously found a false and this one is the second one
				mask_any_false = true;
				break;
			}
			free_index = i;
		}
	}
	if (free_index == buffer_len)
		return;

	mask_any_true = true;
	mask[free_index] = true;

	tag[free_index] = _tag;
	loc[free_index] = _loc;
	comment[free_index] = _comment;
	pin[free_index] = _pin;
	state[free_index] = _state;
}

bool SerialManager::talk_start() {
	pinMode(signal_line, INPUT);
	if (digitalRead(signal_line))
		return false;

	uint32_t now = millis();
	while (millis() - now <= delay_before_send)
		if (digitalRead(signal_line))
			return false;

	if (digitalRead(signal_line))
		return false;

	pinMode(signal_line, OUTPUT);
	digitalWrite(signal_line, HIGH);
	return true;
}

void SerialManager::talk_end() {
	pinMode(signal_line, OUTPUT);
	digitalWrite(signal_line, LOW);
}

void SerialManager::handle_sends() {
	if (!mask_any_true || !talk_start())
		return;

	for (byte i = 0; i < buffer_len; i++) {
		if (!mask[i])
			continue;

		send(
				String(tag[i]) + spacer
				+ String(loc[i]) + spacer
				+ String(comment[i]) + spacer
				+ String(pin[i]) + spacer
				+ String(state[i])
		);

		mask_any_false = true;
		mask[i] = false;
		tag[i] = 0;
		loc[i] = 0;
		comment[i] = 0;
		pin[i] = 0;
		state[i] = 0;
	}

	mask_any_true = false;
	digitalWrite(13, LOW);
	talk_end();
}

void SerialManager::handle_receives() {
	if (!receiver_callback || !Serial.available())
		return;

	String data = Serial.readStringUntil(end);
	if (!data.startsWith(MID))
		return;

	receiver_callback(remove_first_match(data, MID));

}