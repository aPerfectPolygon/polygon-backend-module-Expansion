#ifndef EXPANSIONMODULE_CONFIGS_H
#define EXPANSIONMODULE_CONFIGS_H

#include <Arduino.h>

#define DEBUG true
#define VERSIONS_V0_1 "V0.1|"

const PROGMEM static byte delay_before_send = 10;  // unique to each module
const PROGMEM static byte filter_tags_above = 1;

const static char *spacer = "|";
const static char *MV = VERSIONS_V0_1;  // must have the spacer at the end
const static char *MID = "IO|1|";  // must have the spacer at the end
const static char *MID_MV = "IO|1|V0.1|";  // must have the spacer at the end

#endif //EXPANSIONMODULE_CONFIGS_H
