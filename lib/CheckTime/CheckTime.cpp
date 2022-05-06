#include "CheckTime.h"

CheckTime::CheckTime(uint32_t _last_check, int _check_interval) {
	last_check = _last_check;
	check_interval = _check_interval;
}

bool CheckTime::check(uint32_t now, bool auto_now) {
	if (first_time) {
		first_time = false;
		last_check = now;
		return true;
	}
	if (auto_now)
		now = millis();
	if (now - last_check < check_interval)
		return false;
	last_check = now;
	return true;
}

