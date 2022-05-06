#ifndef EXPANSIONMODULE_SERIALMANAGER_H
#define EXPANSIONMODULE_SERIALMANAGER_H

#include "../../include/Configs.h"
#include "../StringHelper/StringHelper.h"

// region tags
#define TAGS_WARNING 0
#define TAGS_NONE 1
#define TAGS_ERROR 2
#define TAGS_CRITICAL 3
#define TAGS_OUTPUT_CHANGED 4
#define TAGS_INPUT_CHANGED 5
#define TAGS_CONFIG_NEEDED 6
#define TAGS_OK 7
#define TAGS_READ_INPUT 8
#define TAGS_READ_OUTPUT 9
// endregion

// region locs
#define LOCS_NONE 0
#define LOCS_LISTBYTE_INIT 1
#define LOCS_LISTBYTE_INIT_MASK 2
#define LOCS_LISTBYTE_INIT_SM 3
#define LOCS_LISTBYTE_GETINDEX 4
#define LOCS_LISTBYTE_CONTAINS 5
#define LOCS_LISTBYTE_APPEND 6
#define LOCS_LISTBYTE_PUT 7
#define LOCS_LISTBYTE_POP 8
#define LOCS_LISTBYTE_REMOVE 9
#define LOCS_LISTBYTE_REPLACE 10
#define LOCS_LISTBYTE_FIND 11
#define LOCS_LISTBOOL_INIT 12
#define LOCS_LISTBOOL_INIT_MASK 13
#define LOCS_LISTBOOL_INIT_SM 14
#define LOCS_LISTBOOL_GETINDEX 15
#define LOCS_LISTBOOL_CONTAINS 16
#define LOCS_LISTBOOL_APPEND 17
#define LOCS_LISTBOOL_PUT 18
#define LOCS_LISTBOOL_POP 19
#define LOCS_LISTBOOL_REMOVE 20
#define LOCS_LISTBOOL_REPLACE 21
#define LOCS_LISTBOOL_FIND 22
#define LOCS_LISTMASK_INIT 23
#define LOCS_LISTMASK_INIT_SM 24
#define LOCS_LISTMASK_GETINDEX 25
#define LOCS_LISTMASK_CONTAINS 26
#define LOCS_LISTMASK_PUT 27
#define LOCS_LISTMASK_FIRST_FALSE 28
#define LOCS_LISTMASK_FIRST_TRUE 29
#define LOCS_IOMANAGER_DEBOUNCE_HANDLER_CALLBACK 30
#define LOCS_IOMANAGER_INPUT_ADD 31
#define LOCS_IOMANAGER_INPUT_READ 32
#define LOCS_IOMANAGER_INPUT_REMOVE 33
#define LOCS_IOMANAGER_OUTPUT_ADD 34
#define LOCS_IOMANAGER_OUTPUT_READ 35
#define LOCS_IOMANAGER_OUTPUT_SET 36
#define LOCS_IOMANAGER_OUTPUT_REMOVE 37
#define LOCS_IOMANAGER_IOMANAGER 38
#define LOCS_DEBOUNCEHANDLER_INIT_SM 39
#define LOCS_DEBOUNCEHANDLER_ADD 40
#define LOCS_SM_RECEIVER_CALLBACK_IA 42
#define LOCS_SM_RECEIVER_CALLBACK_RI 43
#define LOCS_SM_RECEIVER_CALLBACK_RO 44
// endregion

// region comments
#define COMMENTS_NONE 0
#define COMMENTS_ALREADY_INIT 1
#define COMMENTS_ALREADY_MASK_INIT 2
#define COMMENTS_NOT_INIT 3
#define COMMENTS_NOT_MASK_INIT 4
#define COMMENTS_INCOMPATIBLE_SIZE 5
#define COMMENTS_ALREADY_SM_INIT 6
#define COMMENTS_INDEX_OUT_OF_RANGE 7
#define COMMENTS_NO_MASK_AND_DEFAULT_VALUE 8
#define COMMENTS_OVERFLOW 9
#define COMMENTS_NOT_FOUND 10
#define COMMENTS_NOT_AVAILABLE 11
#define COMMENTS_NOT_ANALOG 12
#define COMMENTS_ALREADY_INPUT 13
#define COMMENTS_OVERWRITING 14
#define COMMENTS_ALREADY_OUTPUT 15
// endregion

class SerialManager {
protected:
	const PROGMEM static byte signal_line = 19;
	const PROGMEM static byte buffer_len = 100;
	const PROGMEM static int baud_rate = 9600;
	const PROGMEM char end = '\n';
	bool mask_any_true = false;
	bool mask_any_false = true;
	bool mask[buffer_len]{};
	byte tag[buffer_len]{};
	uint16_t loc[buffer_len]{};
	uint16_t comment[buffer_len]{};
	byte pin[buffer_len]{};
	byte state[buffer_len]{};
public:
	void (*receiver_callback)(String data) = nullptr;

	void begin();

	void print(const String &t);

	void send(const String &t);

	void log(
			byte tag = TAGS_NONE,
			uint16_t loc = LOCS_NONE,
			uint16_t _comment = COMMENTS_NONE,
			byte _pin = 0,
			byte _state = 0
	);

	bool talk_start();

	static void talk_end();

	void handle_sends();

	void handle_receives();
};

#endif //EXPANSIONMODULE_SERIALMANAGER_H
