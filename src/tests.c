// The test results are in:
// You ARE the father

#include <gccore.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "utils.h"
#include "globals.h"

void die(const char* msg) {
	CONSOLE_RESET();
	printf("The OpenMii Installer couldn't continue due to a problem:\n\n%s\n\n", msg);
	printf("Press any button to return to the Wii Menu.");
	wait_for_pad();
	WII_ReturnToMenu();
}

u32 wait_for_pad( void ) {
	u32 pressed_g = 0;
	u32 pressed_w = 0;

	while (1) {
		WPAD_ScanPads();
		PAD_ScanPads();

		pressed_g = PAD_ButtonsDown(0);
		pressed_w = WPAD_ButtonsDown(0);

		if (pressed_g || pressed_w) break;
	}

	if ((pressed_g == PAD_BUTTON_A) || (pressed_w == WPAD_BUTTON_A)) {
	   return BUTTON_A;
  	}

	if ((pressed_g == PAD_BUTTON_B) || (pressed_w == WPAD_BUTTON_B)) {
		return BUTTON_B;
	}

	if ((pressed_g == PAD_BUTTON_START) || (pressed_w == WPAD_BUTTON_HOME)) {
		return BUTTON_START;
	}

	if ((pressed_g == PAD_BUTTON_UP) || (pressed_w == WPAD_BUTTON_UP)) {
		return BUTTON_UP;
	}

	if ((pressed_g == PAD_BUTTON_DOWN) || (pressed_w == WPAD_BUTTON_DOWN)) {
		return BUTTON_DOWN;
	}

	return BUTTON_UNK;
}
