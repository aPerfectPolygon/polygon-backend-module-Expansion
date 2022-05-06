#ifndef EXPANSIONMODULE_CHECKTIME_H
#define EXPANSIONMODULE_CHECKTIME_H

#include "../../include/Configs.h"

class CheckTime {
private:
	uint32_t last_check;
	int check_interval;
	bool first_time = true;
public:
	explicit CheckTime(uint32_t _last_check, int _check_interval);

	bool check(uint32_t now = 0, bool auto_now = true);
};


#endif //EXPANSIONMODULE_CHECKTIME_H
