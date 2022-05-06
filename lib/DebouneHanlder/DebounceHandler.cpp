#include "DebounceHandler.h"

void DebounceHandler::add(byte _pin, byte _state, bool _is_analog) {
	if (!mask.any_false) {
		if (sm) sm->log(TAGS_ERROR, LOCS_DEBOUNCEHANDLER_ADD, COMMENTS_OVERFLOW);
		return;
	}

	uint8_t free_id = mask.first_false();
	mask.put(free_id, true);
	pin[free_id] = _pin;
	state[free_id] = _state;
	is_analog[free_id] = _is_analog;
	time[free_id] = millis();
}

void DebounceHandler::init_sm(SerialManager *_sm) {
	if (sm) {
		sm->log(TAGS_WARNING, LOCS_DEBOUNCEHANDLER_INIT_SM, COMMENTS_ALREADY_SM_INIT);
		return;
	}
	sm = _sm;
}

void DebounceHandler::handle() {
	if (!mask.any_true)
		// nothing to check
		return;

	uint32_t now = millis();

	for (byte i = 0; i < max_concurrent; i++) {
		if (!mask.arr[i])
			continue;

		if (now - time[i] >= debounce) {  // time to check the pin
			bool fake;
			if (is_analog[i])
				fake = analog_read(pin[i]) < state[i];  // todo handle cross upwards/downwards
			else
				fake = digital_read(pin[i]) != state[i];

			if (!fake && callback)
				callback(pin[i], state[i]);

			// empty the queue
			mask.put(i, false);
			pin[i] = 0;
			time[i] = 0;
			state[i] = 0;
			is_analog[i] = false;

			if (!mask.any_true)
				// there is no more to check
				return;
		}
	}
}