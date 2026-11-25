#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "utils.h"
#include "3rdparty/haxx.h"
#include "hbc.h"
#include "3rdparty/runtimeiospatch.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void libogc_init( void ) {

	VIDEO_Init();
	PAD_Init();
	WPAD_Init();

	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(false);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	printf("\x1b[2;0H");
}

int main( void ) {
	libogc_init();
	CON_EnableGecko(1, true);

	if (!Haxx_GetBusAccess()) {
		die("Failed to get bus access!");
	}
	Restore_Trucha();

	printf("Welcome to the OpenMii Installer\n");
	printf("waiting for pad");
	
	wait_for_pad();

	install_hbc();

	printf("good luck");

	wait_for_pad();

	exit(0);
}
