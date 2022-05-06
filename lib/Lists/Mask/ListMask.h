#ifndef EXPANSIONMODULE_LISTMASK_H
#define EXPANSIONMODULE_LISTMASK_H

#include "../../Utils/utils.h"

class ListMask {
public:
	uint16_t size = 0;
	bool default_value = false;
	bool *arr = nullptr;
	SerialManager *sm = nullptr;
	bool any_true = false;
	bool any_false = true;

	explicit ListMask(uint16_t _size = 0, const bool _default_arr[] = nullptr, bool _default = false);

	void init(uint16_t _size, const bool _default_arr[] = nullptr, bool _default = false);

	void init_sm(SerialManager *_sm);

	bool getIndex(uint16_t _index);

	bool contains(bool item);

	void put(uint16_t _index, bool item);

	uint16_t first_false();

	uint16_t first_true();
};

#endif //EXPANSIONMODULE_LISTMASK_H
