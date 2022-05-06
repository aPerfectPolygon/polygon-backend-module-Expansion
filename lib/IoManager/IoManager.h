#ifndef EXPANSIONMODULE_IOMANAGER_H
#define EXPANSIONMODULE_IOMANAGER_H

#include "../Utils/utils.h"
#include "../Lists/Bool/ListBool.h"
#include "../Lists/Byte/ListByte.h"
#include "../DebouneHanlder/DebounceHandler.h"

class IoManager {
private:
	const static byte default_output_state_digital = 100;
	const static byte default_output_state_analog = 100;
	const static byte scan_interval = 50;  // must be greater than debounce_handler.debounce

	DebounceHandler *debounce_handler = nullptr;
	SerialManager *sm = nullptr;

	CheckTime check_time = CheckTime(0, scan_interval);
public:
	ListByte *io_pins;
	ListByte *ai_pins;
	ListByte *ao_pins;
	uint16_t inputs_len;
	uint16_t outputs_len;

	ListByte inputs;
	ListByte inputs_state;
	ListMask inputs_initialized;
	ListBool inputs_is_analog;
	ListByte outputs;
	ListByte outputs_state;
	ListMask outputs_initialized;
	ListBool outputs_is_analog;

	void (*callback_input_changed)(byte pin, byte state, bool is_analog) = nullptr;

	void (*callback_output_changed)(byte pin, byte state) = nullptr;

	explicit IoManager(
			ListByte *_io_pins,
			ListByte *_ai_pins,
			ListByte *_ao_pins,
			uint16_t _inputs_len,
			uint16_t _outputs_len,
			DebounceHandler *_debounce_handler
	);

	void init_sm(SerialManager *_sm);

	void print_variables(const String &tag = "") const;

	uint16_t input_add(byte pin, bool is_analog = false);

	byte input_read(byte pin);

	void input_remove(uint16_t _index);

	void input_remove(byte pin);

	uint16_t output_add(byte pin, bool is_analog = false);

	byte output_read(byte pin);

	void output_remove(uint16_t _index);

	void output_remove(byte pin);

	void output_set(byte pin, byte state);

	void scan();

	void debounce_handler_callback(byte pin, byte state);
};

#endif //EXPANSIONMODULE_IOMANAGER_H
