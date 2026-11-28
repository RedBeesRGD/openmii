// The test results are in:
// You ARE the father

#include <gccore.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "utils.h"
#include "globals.h"
#include "3rdparty/otp.h"
#include "tests.h"

u8 get_console_type() {
	u8 otp_data[128];

	otp_read(&otp_data, 0x14, 0x10);

	switch (otp_data[0]) {
		case 0xEB:
			return CONSOLE_RETAIL;
		case 0xA1:
			return CONSOLE_DEVEL;
		default:
			return CONSOLE_UNK;
	}
}
