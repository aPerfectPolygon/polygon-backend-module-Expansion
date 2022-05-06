#ifndef EXPANSIONMODULE_DEBOUNCEHANDLER_H
#define EXPANSIONMODULE_DEBOUNCEHANDLER_H

#include "../Utils/utils.h"
#include "../Lists/Mask/ListMask.h"

class DebounceHandler {
private:
	static const byte debounce = 20;
	static const byte max_concurrent = 20;  // each one consumes 8 byte of memory
	SerialManager *sm = nullptr;
	ListMask mask = ListMask(max_concurrent);
	byte pin[max_concurrent]{};
	byte state[max_concurrent]{};
	uint32_t time[max_concurrent]{};
	bool is_analog[max_concurrent]{};
public:
	void (*callback)(byte pin, byte state) = nullptr;

	void add(byte pin, byte state, bool is_analog = false);

	void init_sm(SerialManager *_sm);

	void handle();
};

#endif //EXPANSIONMODULE_DEBOUNCEHANDLER_H
