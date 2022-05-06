#include "IoManager.h"

IoManager::IoManager(
		ListByte *_io_pins,
		ListByte *_ai_pins,
		ListByte *_ao_pins,
		uint16_t _inputs_len,
		uint16_t _outputs_len,
		DebounceHandler *_debounce_handler
) {
	if (!_io_pins->arr || !_ai_pins->arr || !_ao_pins->arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_IOMANAGER, COMMENTS_NOT_INIT);
		return;
	}

	io_pins = _io_pins;
	ai_pins = _ai_pins;
	ao_pins = _ao_pins;

	inputs_len = _inputs_len;
	outputs_len = _outputs_len;

	inputs.init(inputs_len);
	inputs_state.init(inputs_len);
	inputs_is_analog.init(inputs_len);
	inputs_initialized.init(inputs_len);
	inputs.init_mask(inputs_initialized);
	inputs_state.init_mask(inputs_initialized);
	inputs_is_analog.init_mask(inputs_initialized);

	outputs.init(outputs_len);
	outputs_state.init(outputs_len);
	outputs_is_analog.init(outputs_len);
	outputs_initialized.init(outputs_len);
	outputs.init_mask(outputs_initialized);
	outputs_state.init_mask(outputs_initialized);
	outputs_is_analog.init_mask(outputs_initialized);

	debounce_handler = _debounce_handler;
}

void IoManager::init_sm(SerialManager *_sm) {

	io_pins->init_sm(_sm);
	ai_pins->init_sm(_sm);
	ao_pins->init_sm(_sm);
	inputs.init_sm(_sm);
	inputs_state.init_sm(_sm);
	inputs_initialized.init_sm(_sm);
	inputs_is_analog.init_sm(_sm);
	outputs.init_sm(_sm);
	outputs_state.init_sm(_sm);
	outputs_initialized.init_sm(_sm);
	outputs_is_analog.init_sm(_sm);
	sm = _sm;
}

void IoManager::debounce_handler_callback(byte pin, byte state) {
	byte pin_index = inputs.find(pin);
	if (pin_index == inputs_len) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_DEBOUNCE_HANDLER_CALLBACK, COMMENTS_NOT_FOUND, pin);
		return;
	}

	inputs_state.arr[pin_index] = state;
	if (callback_input_changed)
		callback_input_changed(pin, state, false);
}

void IoManager::print_variables(const String &tag) const {
	if (!DEBUG)
		return;
	const PROGMEM char *empty_ending = "";
	const PROGMEM char *sep = ", ";
	const PROGMEM char *item_end = ",\n\t";

	if (tag != "")
		print(tag);

	print(F("Inputs: \n\t"), empty_ending);
	for (byte i = 0; i < inputs_len; i++) {
		print(String(inputs_initialized.arr[i]) + sep
		      + String(inputs_is_analog.arr[i]) + sep
		      + String(inputs.arr[i]) + sep
		      + String(inputs_state.arr[i])
		      + item_end,
		      empty_ending
		);
	}

	print(F("\nOutputs:\n\t"), empty_ending);
	for (byte i = 0; i < outputs_len; i++) {
		print(String(outputs_initialized.arr[i]) + sep
		      + String(outputs_is_analog.arr[i]) + sep
		      + String(outputs.arr[i]) + sep
		      + String(outputs_state.arr[i])
		      + item_end,
		      empty_ending
		);
	}
	print(empty_ending);
}

uint16_t IoManager::input_add(byte pin, bool is_analog) {
	if (!io_pins->contains(pin)) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_INPUT_ADD, COMMENTS_NOT_AVAILABLE, pin);
		return inputs_len;
	}
	if (is_analog && !ai_pins->contains(pin)) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_INPUT_ADD, COMMENTS_NOT_ANALOG, pin);
		return inputs_len;
	}

	// if pin is already configured as input do nothing
	uint16_t input_id = inputs.find(pin);
	if (input_id != inputs_len && inputs_is_analog.arr[input_id] == is_analog) {
		if (sm) sm->log(TAGS_WARNING, LOCS_IOMANAGER_INPUT_ADD, COMMENTS_ALREADY_INPUT, pin);
		return input_id;
	}

	// if pin is already configured as output overwrite it
	uint16_t output_id = outputs.find(pin);
	if (output_id != outputs_len) {
		if (sm) sm->log(TAGS_WARNING, LOCS_IOMANAGER_INPUT_ADD, COMMENTS_OVERWRITING, pin);
		output_remove(output_id);
	}

	input_id = inputs.append(pin);
	if (input_id == inputs_len)
		return input_id;

	pinMode(pin, INPUT_PULLUP);
	inputs_is_analog.arr[input_id] = is_analog;
	if (is_analog)
		inputs_state.arr[input_id] = analog_read(pin);
	else
		inputs_state.arr[input_id] = digital_read(pin);

	return input_id;
}

byte IoManager::input_read(byte pin) {
	uint16_t pin_index = inputs.find(pin);
	if (pin_index == inputs_len) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_INPUT_READ, COMMENTS_NOT_FOUND, pin);
		return 0;
	}

	return inputs_state.arr[pin_index];
}

void IoManager::input_remove(byte pin) {
	uint16_t pin_index = inputs.find(pin);
	if (pin_index != inputs_len) {
		input_remove(pin_index);
	} else {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_INPUT_REMOVE, COMMENTS_NOT_FOUND, pin);
	}
}

void IoManager::input_remove(uint16_t _index) {
	if (_index >= inputs_len) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_INPUT_REMOVE, COMMENTS_INDEX_OUT_OF_RANGE);
	}

	inputs.pop(_index);
	inputs_is_analog.arr[_index] = inputs_is_analog.default_value;
	inputs_state.arr[_index] = inputs_state.default_value;
}

uint16_t IoManager::output_add(byte pin, bool is_analog) {
	if (!io_pins->contains(pin)) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_OUTPUT_ADD, COMMENTS_NOT_AVAILABLE, pin);
		return outputs_len;
	}
	if (is_analog && !ao_pins->contains(pin)) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_OUTPUT_ADD, COMMENTS_NOT_ANALOG, pin);
		return outputs_len;
	}

	// if pin is already configured as output do nothing
	uint16_t output_id = outputs.find(pin);
	if (output_id != outputs_len && outputs_is_analog.arr[output_id] == is_analog) {
		if (sm) sm->log(TAGS_WARNING, LOCS_IOMANAGER_OUTPUT_ADD, COMMENTS_ALREADY_OUTPUT, pin);
		return output_id;
	}

	// if pin is already configured as input overwrite it
	uint16_t input_id = inputs.find(pin);
	if (input_id != inputs_len) {
		if (sm) sm->log(TAGS_WARNING, LOCS_IOMANAGER_OUTPUT_ADD, COMMENTS_OVERWRITING, pin);
		input_remove(input_id);
	}

	output_id = outputs.append(pin);
	if (output_id == outputs_len)
		return output_id;

	pinMode(pin, OUTPUT);
	outputs_is_analog.arr[output_id] = is_analog;
	if (is_analog) {
		outputs_state.arr[output_id] = default_output_state_analog;
		analog_write(pin, default_output_state_analog);
	} else {
		outputs_state.arr[output_id] = default_output_state_digital;
		digital_write(pin, default_output_state_digital);
	}

	return output_id;
}

byte IoManager::output_read(byte pin) {
	uint16_t pin_index = outputs.find(pin);
	if (pin_index == outputs_len) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_OUTPUT_READ, COMMENTS_NOT_FOUND, pin);
		return 0;
	}

	return outputs_state.arr[pin_index];
}

void IoManager::output_set(byte pin, byte state) {
	uint16_t pin_index = outputs.find(pin);
	if (pin_index == outputs_len) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_OUTPUT_SET, COMMENTS_NOT_FOUND, pin);
		return;
	}

	if (outputs_is_analog.arr[pin_index]) {
		analog_write(pin, state);
	} else {
		if (state != 0)
			state = 100;
		digital_write(pin, state);
	}

	if (outputs_state.arr[pin_index] != state) {
		outputs_state.arr[pin_index] = state;
		if (callback_output_changed)
			callback_output_changed(pin, state);
	}
}

void IoManager::output_remove(byte pin) {
	uint16_t pin_index = outputs.find(pin);
	if (pin_index != outputs_len) {
		output_remove(pin_index);
	} else {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_OUTPUT_REMOVE, COMMENTS_NOT_FOUND, pin);
	}
}

void IoManager::output_remove(uint16_t _index) {
	if (_index >= outputs_len) {
		if (sm) sm->log(TAGS_ERROR, LOCS_IOMANAGER_OUTPUT_REMOVE, COMMENTS_INDEX_OUT_OF_RANGE);
	}

	outputs.pop(_index);
	outputs_is_analog.arr[_index] = outputs_is_analog.default_value;
	outputs_state.arr[_index] = outputs_state.default_value;
}

void IoManager::scan() {
	if (!inputs_initialized.any_true || !check_time.check())
		return;

	for (uint16_t i = 0; i < inputs_len; i++) {
		if (!inputs_initialized.arr[i])
			continue;

		byte new_state;
		if (inputs_is_analog.arr[i]) {
			new_state = analog_read(inputs.arr[i]);
			if (new_state != inputs_state.arr[i]) {
				inputs_state.arr[i] = new_state;
				if (callback_input_changed)
					callback_input_changed(inputs.arr[i], new_state, true);
			}
		} else {
			new_state = digital_read(inputs.arr[i]);
			if (new_state != inputs_state.arr[i]) {
				debounce_handler->add(inputs.arr[i], new_state);
			}
		}
	}
}
