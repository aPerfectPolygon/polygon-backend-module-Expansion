#include "ListByte.h"

ListByte::ListByte(uint16_t _size, const byte _default_arr[], byte _default) {
	if (_size != 0)
		init(_size, _default_arr, _default);
}

void ListByte::init(uint16_t _size, const byte _default_arr[], byte _default) {
	if (arr) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_INIT, COMMENTS_ALREADY_INIT);
		return;
	}

	size = _size;
	default_value = _default;

	arr = (byte *) malloc(size);

	if (!_default_arr) {
		for (uint16_t i = 0; i < size; i++)
			arr[i] = default_value;
	} else {
		for (uint16_t i = 0; i < size; i++)
			arr[i] = _default_arr[i];
	}
}

void ListByte::init_mask(ListMask &_mask) {
	if (mask) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_INIT_MASK, COMMENTS_ALREADY_MASK_INIT);
		return;
	}
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_INIT_MASK, COMMENTS_NOT_INIT);
		return;
	}
	if (!_mask.arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_INIT_MASK, COMMENTS_NOT_MASK_INIT);
		return;
	}
	if (_mask.size != size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_INIT_MASK, COMMENTS_INCOMPATIBLE_SIZE);
		return;
	}
	mask = &_mask;
}

void ListByte::init_sm(SerialManager *_sm) {
	if (sm) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_INIT_SM, COMMENTS_ALREADY_SM_INIT);
		return;
	}
	sm = _sm;
}

byte ListByte::getIndex(uint16_t _index) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_GETINDEX, COMMENTS_NOT_INIT);
		return default_value;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_GETINDEX, COMMENTS_INDEX_OUT_OF_RANGE);
		return default_value;
	}

	return arr[_index];
}

bool ListByte::contains(byte item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_CONTAINS, COMMENTS_NOT_INIT);
		return false;
	}

	if (mask) {
		for (uint16_t i = 0; i < size; i++)
			if (mask->arr[i] && arr[i] == item)
				return true;
	} else {
		if (item == default_value)
			if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_CONTAINS, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++)
			if (arr[i] == item)
				return true;
	}
	return false;
}

uint16_t ListByte::append(byte item) {
	if (!mask) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_APPEND, COMMENTS_NOT_MASK_INIT);
		return size;
	}
	if (!mask->any_false) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_APPEND, COMMENTS_OVERFLOW);
		return size;
	}

	uint16_t free_index = mask->first_false();
	mask->put(free_index, true);
	arr[free_index] = item;
	return free_index;
}

void ListByte::put(uint16_t _index, byte item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_PUT, COMMENTS_NOT_INIT);
		return;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_PUT, COMMENTS_INDEX_OUT_OF_RANGE);
		return;
	}

	if (mask)
		mask->put(_index, true);
	arr[_index] = item;
}

byte ListByte::pop(uint16_t _index) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_POP, COMMENTS_NOT_INIT);
		return default_value;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_POP, COMMENTS_INDEX_OUT_OF_RANGE);
		return default_value;
	}

	byte _val = arr[_index];
	if (mask)
		mask->put(_index, false);
	arr[_index] = default_value;
	return _val;
}

uint16_t ListByte::remove(byte item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_REMOVE, COMMENTS_NOT_INIT);
		return size;
	}
	uint16_t _index = size;

	if (mask) {
		for (uint16_t i = 0; i < size; i++) {
			if (mask->arr[i] && arr[i] == item) {
				_index = i;
				break;
			}
		}
	} else {
		if (item == default_value)
			if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_REMOVE, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++) {
			if (arr[i] == item) {
				_index = i;
				break;
			}
		}
	}

	if (_index == size) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_REMOVE, COMMENTS_NOT_FOUND);
	} else {
		if (mask)
			mask->put(_index, false);
		arr[_index] = default_value;
	}

	return _index;
}

uint16_t ListByte::replace(byte item, byte new_item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_REPLACE, COMMENTS_NOT_INIT);
		return size;
	}
	uint16_t _index = size;

	if (mask) {
		for (uint16_t i = 0; i < size; i++) {
			if (mask->arr[i] && arr[i] == item) {
				_index = i;
				break;
			}
		}
	} else {
		if (item == default_value)
			if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_REPLACE, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++) {
			if (arr[i] == item) {
				_index = i;
				break;
			}
		}
	}

	if (_index == size) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_REPLACE, COMMENTS_NOT_FOUND);
	} else {
		arr[_index] = new_item;
	}

	return _index;
}

uint16_t ListByte::find(byte item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBYTE_FIND, COMMENTS_NOT_INIT);
		return size;
	}
	uint16_t _index = size;

	if (mask) {
		for (uint16_t i = 0; i < size; i++) {
			if (mask->arr[i] && arr[i] == item) {
				_index = i;
				break;
			}
		}
	} else {
		if (item == default_value)
			if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_FIND, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++) {
			if (arr[i] == item) {
				_index = i;
				break;
			}
		}
	}

	if (_index == size)
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBYTE_FIND, COMMENTS_NOT_FOUND);

	return _index;
}
