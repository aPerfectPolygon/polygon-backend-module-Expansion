#include "StringHelper.h"

String remove_first_match(String find_in, const String &search_for) {
	int index = find_in.indexOf(search_for);

	if (index != -1)
		return find_in.substring(index + search_for.length());
	return find_in;
}