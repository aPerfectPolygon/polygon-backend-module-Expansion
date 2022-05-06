#include "../lib/Utils/utils.h"
#include "../lib/DebouneHanlder/DebounceHandler.h"
#include "../lib/IoManager/IoManager.h"

SerialManager sm;
DebounceHandler debounce_handler;
ListByte io_pins(16);
ListByte ai_pins(5);
ListByte ao_pins(6);
IoManager io_manager(&io_pins, &ai_pins, &ao_pins, 16, 16, &debounce_handler);

void output_changed(byte pin, byte state) {
	sm.log(TAGS_OUTPUT_CHANGED, LOCS_NONE, COMMENTS_NONE, pin, state);
}

void input_changed(byte pin, byte state, bool is_analog) {
	sm.log(TAGS_INPUT_CHANGED, LOCS_NONE, COMMENTS_NONE, pin, state);
}

void debounce_handler_callback(byte pin, byte state) {
	io_manager.debounce_handler_callback(pin, state);
}

void sm_receiver_callback(String data) {
	if (data == F("RA")) {
		io_manager.print_variables("RA");
	} else if (data == F("IA")) {
		sm.log(TAGS_OK, LOCS_SM_RECEIVER_CALLBACK_IA);
	} else if (data.startsWith(F("RI"))) {
		// RI
		// RI1,2,3
		data = data.substring(2);
		if (data == "") {
			for (uint32_t i = 0; i < io_manager.inputs_initialized.size; i++) {
				if (io_manager.inputs_initialized.arr[i]) {
					sm.log(
							TAGS_READ_INPUT, LOCS_SM_RECEIVER_CALLBACK_RI, COMMENTS_NONE,
							io_manager.inputs.arr[i], io_manager.input_read(io_manager.inputs.arr[i])
					);
				}
			}
		} else {
			while (data != "") {
				int comma_index = data.indexOf(F(","));
				byte pin = data.substring(0, comma_index).toInt();
				if (comma_index == -1)
					data = "";
				else
					data = data.substring(comma_index + 1);

				if (!io_manager.inputs.contains(pin)) {
					sm.log(TAGS_ERROR, LOCS_SM_RECEIVER_CALLBACK_RI, COMMENTS_NOT_FOUND, pin);
				} else {
					sm.log(
							TAGS_READ_INPUT, LOCS_SM_RECEIVER_CALLBACK_RI,
							COMMENTS_NONE, pin, io_manager.input_read(pin)
					);
				}
			}
		}
	} else if (data.startsWith(F("RO"))) {
		data = data.substring(2);
		if (data == "") {
			for (uint32_t i = 0; i < io_manager.outputs_initialized.size; i++) {
				if (io_manager.outputs_initialized.arr[i]) {
					sm.log(
							TAGS_READ_OUTPUT, LOCS_SM_RECEIVER_CALLBACK_RO, COMMENTS_NONE,
							io_manager.outputs.arr[i], io_manager.output_read(io_manager.outputs.arr[i])
					);
				}
			}
		} else {
			while (data != "") {
				int comma_index = data.indexOf(F(","));
				byte pin = data.substring(0, comma_index).toInt();
				if (comma_index == -1)
					data = "";
				else
					data = data.substring(comma_index + 1);

				if (!io_manager.outputs.contains(pin)) {
					sm.log(TAGS_ERROR, LOCS_SM_RECEIVER_CALLBACK_RO, COMMENTS_NOT_FOUND, pin);
				} else {
					sm.log(
							TAGS_READ_OUTPUT, LOCS_SM_RECEIVER_CALLBACK_RO,
							COMMENTS_NONE, pin, io_manager.output_read(pin)
					);
				}
			}
		}
	} else if (data.startsWith(F("SIO"))) {
		// SIO1=0,2=2
		// states:
		//      0: DigitalOutput
		//      1: AnalogOutput
		//      2: DigitalInput
		//      3: AnalogInput
		data = data.substring(3);
		while (data != "") {
			int eq_index = data.indexOf(F("="));
			int comma_index = data.indexOf(F(","));

			byte pin = data.substring(0, eq_index).toInt();
			byte state = data.substring(eq_index + 1, comma_index).toInt();

			if (comma_index == -1)
				data = "";
			else
				data = data.substring(comma_index + 1);

			if (state == 0)
				io_manager.output_add(pin, false);
			else if (state == 1)
				io_manager.output_add(pin, true);
			if (state == 2)
				io_manager.input_add(pin, false);
			else if (state == 3)
				io_manager.input_add(pin, true);
		}
	} else if (data.startsWith(F("SO"))) {
		// SO1=100,2=0
		data = data.substring(2);
		while (data != "") {
			int eq_index = data.indexOf(F("="));
			int comma_index = data.indexOf(F(","));

			byte pin = data.substring(0, eq_index).toInt();
			byte state = data.substring(eq_index + 1, comma_index).toInt();

			if (comma_index == -1)
				data = "";
			else
				data = data.substring(comma_index + 1);

			io_manager.output_set(pin, state);
		}
	}
}

void setup() {
	// region io_manager related initial states
	io_pins.arr[0] = 2;
	io_pins.arr[1] = 3;
	io_pins.arr[2] = 4;
	io_pins.arr[3] = 5;
	io_pins.arr[4] = 6;
	io_pins.arr[5] = 7;
	io_pins.arr[6] = 8;
	io_pins.arr[7] = 9;
	io_pins.arr[8] = 10;
	io_pins.arr[9] = 11;
	io_pins.arr[10] = 12;
	io_pins.arr[11] = 14;
	io_pins.arr[12] = 15;
	io_pins.arr[13] = 16;
	io_pins.arr[14] = 17;
	io_pins.arr[15] = 18;

	ai_pins.arr[0] = 14;
	ai_pins.arr[1] = 15;
	ai_pins.arr[2] = 16;
	ai_pins.arr[3] = 17;
	ai_pins.arr[4] = 18;

	ao_pins.arr[0] = 3;
	ao_pins.arr[1] = 4;
	ao_pins.arr[2] = 5;
	ao_pins.arr[3] = 9;
	ao_pins.arr[4] = 10;
	ao_pins.arr[5] = 11;

	// endregion

	sm.receiver_callback = &sm_receiver_callback;
	debounce_handler.callback = &debounce_handler_callback;
	io_manager.callback_output_changed = &output_changed;
	io_manager.callback_input_changed = &input_changed;

	sm.begin();
	io_manager.init_sm(&sm);

	sm.log(TAGS_CONFIG_NEEDED);
}

void loop() {
	io_manager.scan();
	debounce_handler.handle();
	sm.handle_sends();
	sm.handle_receives();
}