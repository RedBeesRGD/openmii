#include <gccore.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "utils.h"

void die(const char* msg) {
	printf("%s\n", msg);
	printf("press any button to exit");
	wait_for_pad();
	exit(EXIT_FAILURE);
}

u32 wait_for_pad( void ) {
	u32 pressed = 0;
	u32 pressed_w = 0;

	while (1) {
		WPAD_ScanPads();
		PAD_ScanPads();

		pressed = PAD_ButtonsDown(0);
		pressed_w = WPAD_ButtonsDown(0);

		if (pressed || pressed_w) break;
	}

	if (pressed) return pressed;

	return pressed_w;
}
