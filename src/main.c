#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "utils.h"
#include "3rdparty/haxx.h"
#include "hbc.h"
#include "3rdparty/runtimeiospatch.h"
#include "globals.h"
#include "3rdparty/pngu.h"
#include "bg.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

s32 bg_init ( void ) {
	IMGCTX ctx = NULL;
	PNGUPROP imgProp;

	s32 ret;

	ctx = PNGU_SelectImageFromBuffer(bg);
	if (!ctx) {
		ret = -1;
		goto out;
	}

	ret = PNGU_GetImageProperties(ctx, &imgProp);
	if (ret != PNGU_OK) {
		ret = -1;
		goto out;
	}

	PNGU_DECODE_TO_COORDS_YCbYCr(ctx, 0, 0, imgProp.imgWidth, imgProp.imgHeight, rmode->fbWidth, rmode->xfbHeight, xfb);

	ret = 0;

out:
	if (ctx) {
		PNGU_ReleaseImageContext(ctx);
	}

	return ret;
}

void libogc_init( void ) {

	VIDEO_Init();
	PAD_Init();

	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
	VIDEO_SetBlack(false);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	CON_InitEx(rmode, CONSOLE_XCOORD, CONSOLE_YCOORD, CONSOLE_WIDTH, CONSOLE_HEIGHT);
	bg_init();
	printf("\x1b[2;0H");
}

int main( void ) {
	libogc_init();
	CON_EnableGecko(1, true);

	IOS_ReloadIOS(IOS_GetVersion()); // clean up from loader

	if (!Haxx_GetBusAccess()) {
		die("Failed to get bus access!");
	}
	if (!Restore_Trucha()) {
		die("Failed to apply Trucha patches!");
	}

	WPAD_Init(); // WPAD should be initialized after patching, apparently

	printf("Welcome to the OpenMii Installer\n");
	printf("waiting for pad\n");
	
	wait_for_pad();

	install_hbc();

	printf("good luck");

	wait_for_pad();

	exit(0);
}
