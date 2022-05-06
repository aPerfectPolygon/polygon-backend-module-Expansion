#include "ListBool.h"

ListBool::ListBool(uint16_t _size, const bool _default_arr[], bool _default) {
	if (_size != 0)
		init(_size, _default_arr, _default);
}

void ListBool::init(uint16_t _size, const bool _default_arr[], bool _default) {
	if (arr) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_INIT, COMMENTS_ALREADY_INIT);
		return;
	}

	size = _size;
	default_value = _default;

	arr = (bool *) malloc(size);

	if (!_default_arr) {
		for (uint16_t i = 0; i < size; i++)
			arr[i] = default_value;
	} else {
		for (uint16_t i = 0; i < size; i++)
			arr[i] = _default_arr[i];
	}
}

void ListBool::init_mask(ListMask &_mask) {
	if (mask) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_INIT_MASK, COMMENTS_ALREADY_MASK_INIT);
		return;
	}
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_INIT_MASK, COMMENTS_NOT_INIT);
		return;
	}
	if (!_mask.arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_INIT_MASK, COMMENTS_NOT_MASK_INIT);
		return;
	}
	if (_mask.size != size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_INIT_MASK, COMMENTS_INCOMPATIBLE_SIZE);
		return;
	}
	mask = &_mask;
}

void ListBool::init_sm(SerialManager *_sm) {
	if (sm) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_INIT_SM, COMMENTS_ALREADY_SM_INIT);
		return;
	}
	sm = _sm;
}

bool ListBool::getIndex(uint16_t _index) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_GETINDEX, COMMENTS_NOT_INIT);
		return default_value;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_GETINDEX, COMMENTS_INDEX_OUT_OF_RANGE);
		return default_value;
	}

	return arr[_index];
}

bool ListBool::contains(bool item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_CONTAINS, COMMENTS_NOT_INIT);
		return false;
	}

	if (mask) {
		for (uint16_t i = 0; i < size; i++)
			if (mask->arr[i] && arr[i] == item)
				return true;
	} else {
		if (item == default_value)
			if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_CONTAINS, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++)
			if (arr[i] == item)
				return true;
	}
	return false;
}

uint16_t ListBool::append(bool item) {
	if (!mask) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_APPEND, COMMENTS_NOT_MASK_INIT);
		return size;
	}
	if (!mask->any_false) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_APPEND, COMMENTS_OVERFLOW);
		return size;
	}

	uint16_t free_index = mask->first_false();
	mask->put(free_index, true);
	arr[free_index] = item;
	return free_index;
}

void ListBool::put(uint16_t _index, bool item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_PUT, COMMENTS_NOT_INIT);
		return;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_PUT, COMMENTS_INDEX_OUT_OF_RANGE);
		return;
	}

	if (mask)
		mask->put(_index, true);
	arr[_index] = item;
}

bool ListBool::pop(uint16_t _index) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_POP, COMMENTS_NOT_INIT);
		return default_value;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_POP, COMMENTS_INDEX_OUT_OF_RANGE);
		return default_value;
	}

	bool _val = arr[_index];
	if (mask)
		mask->put(_index, false);
	arr[_index] = default_value;
	return _val;
}

uint16_t ListBool::remove(bool item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_REMOVE, COMMENTS_NOT_INIT);
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
			if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_REMOVE, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++) {
			if (arr[i] == item) {
				_index = i;
				break;
			}
		}
	}

	if (_index == size) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_REMOVE, COMMENTS_NOT_FOUND);
	} else {
		if (mask)
			mask->put(_index, false);
		arr[_index] = default_value;
	}

	return _index;
}

uint16_t ListBool::replace(bool item, bool new_item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_REPLACE, COMMENTS_NOT_INIT);
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
			if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_REPLACE, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++) {
			if (arr[i] == item) {
				_index = i;
				break;
			}
		}
	}

	if (_index == size) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_REPLACE, COMMENTS_NOT_FOUND);
	} else {
		arr[_index] = new_item;
	}

	return _index;
}

uint16_t ListBool::find(bool item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTBOOL_FIND, COMMENTS_NOT_INIT);
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
			if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_FIND, COMMENTS_NO_MASK_AND_DEFAULT_VALUE);

		for (uint16_t i = 0; i < size; i++) {
			if (arr[i] == item) {
				_index = i;
				break;
			}
		}
	}

	if (_index == size)
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTBOOL_FIND, COMMENTS_NOT_FOUND);

	return _index;
}
