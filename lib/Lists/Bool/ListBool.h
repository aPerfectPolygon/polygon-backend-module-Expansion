#ifndef EXPANSIONMODULE_LISTBOOL_H
#define EXPANSIONMODULE_LISTBOOL_H

#include "../Mask/ListMask.h"

class ListBool {
public:
	uint16_t size = 0;
	bool default_value = false;
	SerialManager *sm = nullptr;
	ListMask *mask = nullptr;
	bool *arr = nullptr;

	explicit ListBool(uint16_t _size = 0, const bool _default_arr[] = nullptr, bool _default = false);

	void init(uint16_t _size, const bool _default_arr[] = nullptr, bool _default = false);

	void init_mask(ListMask &_mask);

	void init_sm(SerialManager *_sm);

	bool getIndex(uint16_t _index);

	bool contains(bool item);

	uint16_t append(bool item);

	void put(uint16_t _index, bool item);

	bool pop(uint16_t _index);

	uint16_t remove(bool item);

	uint16_t replace(bool item, bool new_item);

	uint16_t find(bool item);
};

#endif //EXPANSIONMODULE_LISTBOOL_H
