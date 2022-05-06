#include "ListMask.h"

ListMask::ListMask(uint16_t _size, const bool _default_arr[], bool _default) {
	if (_size != 0)
		init(_size, _default_arr, _default);
}

void ListMask::init(uint16_t _size, const bool _default_arr[], bool _default) {
	if (arr) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTMASK_INIT, COMMENTS_ALREADY_INIT);
		return;
	}

	size = _size;
	default_value = _default;

	arr = (bool *) malloc(sizeof(bool) * size);

	if (!_default_arr) {
		for (uint16_t i = 0; i < size; i++)
			arr[i] = default_value;
	} else {
		for (uint16_t i = 0; i < size; i++)
			arr[i] = _default_arr[i];
	}
}

void ListMask::init_sm(SerialManager *_sm) {
	if (sm) {
		if (sm) sm->log(TAGS_WARNING, LOCS_LISTMASK_INIT_SM, COMMENTS_ALREADY_SM_INIT);
		return;
	}
	sm = _sm;
}

bool ListMask::getIndex(uint16_t _index) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_GETINDEX, COMMENTS_NOT_INIT);
		return default_value;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_GETINDEX, COMMENTS_INDEX_OUT_OF_RANGE);
		return default_value;
	}

	return arr[_index];
}

bool ListMask::contains(bool item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_CONTAINS, COMMENTS_NOT_INIT);
		return false;
	}

	for (uint16_t i = 0; i < size; i++)
		if (arr[i] == item)
			return true;
	return false;
}

void ListMask::put(uint16_t _index, bool item) {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_PUT, COMMENTS_NOT_INIT);
		return;
	}
	if (_index >= size) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_PUT, COMMENTS_INDEX_OUT_OF_RANGE);
		return;
	}

	arr[_index] = item;
	if (item) {
		any_true = true;
		if (!contains(false)) any_false = false;
	} else {
		any_false = true;
		if (!contains(true)) any_true = false;
	}
}

uint16_t ListMask::first_false() {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_FIRST_FALSE, COMMENTS_NOT_INIT);
		return size;
	}
	if (!any_false) {
		return size;
	}

	for (uint16_t i = 0; i < size; i++)
		if (!arr[i])
			return i;
	return size;
}

uint16_t ListMask::first_true() {
	if (!arr) {
		if (sm) sm->log(TAGS_ERROR, LOCS_LISTMASK_FIRST_TRUE, COMMENTS_NOT_INIT);
		return size;
	}
	if (!any_true) {
		return size;
	}

	for (uint16_t i = 0; i < size; i++)
		if (arr[i])
			return i;
	return size;
}

