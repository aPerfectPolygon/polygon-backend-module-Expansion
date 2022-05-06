#ifndef EXPANSIONMODULE_LISTBYTE_H
#define EXPANSIONMODULE_LISTBYTE_H

#include "../Mask/ListMask.h"

class ListByte {
public:
	uint16_t size = 0;
	byte default_value = 0;
	SerialManager *sm = nullptr;
	ListMask *mask = nullptr;
	byte *arr = nullptr;

	explicit ListByte(uint16_t _size = 0, const byte _default_arr[] = nullptr, byte _default = 0);

	void init(uint16_t _size, const byte _default_arr[] = nullptr, byte _default = 0);

	void init_mask(ListMask &_mask);

	void init_sm(SerialManager *_sm);

	byte getIndex(uint16_t _index);

	bool contains(byte item);

	uint16_t append(byte item);

	void put(uint16_t _index, byte item);

	byte pop(uint16_t _index);

	uint16_t remove(byte item);

	uint16_t replace(byte item, byte new_item);

	uint16_t find(byte item);
};

#endif //EXPANSIONMODULE_LISTBYTE_H
