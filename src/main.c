#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <string.h>

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void libogc_init( void ) {

	VIDEO_Init();

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
	printf("hello, openmii");
	while(1) {}
		exit(0);
}
