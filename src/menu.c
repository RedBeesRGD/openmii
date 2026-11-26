#include <gccore.h>
#include <stdlib.h>
#include <wiiuse/wpad.h>

#include "menu.h"
#include "globals.h"
#include "utils.h"
#include "hbc.h"

#define MENU_BASE_ROW 3
#define TEXT_BASE_COL 3

#define OPT_MAX (MenuOpt_Count + (MENU_BASE_ROW-1))

u8 gRow = MENU_BASE_ROW;
u8 gColumn = 0;

typedef enum {
	MenuOpt_InstallHbc = 0,
	MenuOpt_Exit,
	MenuOpt_Count
} MenuOpt;

static const char *menu_opt_str_list[MenuOpt_Count] = {
	"Install the Homebrew Channel",
	"Exit"
};

void cursor_set(u8 row, u8 column) {
	printf("\033[%i;%iH", row, column);
}

void text_newline(void) {
	printf("\n\033[%iC", TEXT_BASE_COL);
}

void subscreen( void ) {
	cursor_set(MENU_BASE_ROW, 0); // clear cursor
	printf("\033[J");

	cursor_set(MENU_BASE_ROW, TEXT_BASE_COL); // clear contents
	printf("\033[J");
}

void handle_opt(u8 opt) {
	switch(opt) {
		case MenuOpt_InstallHbc:
			subscreen();
			printf("Installing the Homebrew Channel...");
			install_hbc();
			text_newline();
			printf("Done.");
			wait_for_pad();
			break;
		case MenuOpt_Exit:
			exit(0);
			break;
		default:
			printf("opt %i\n", opt);
			break;
		}
}



u8 row_to_opt(u8 row) {
	return row - MENU_BASE_ROW;
}

void do_main_menu( void ) {
	while(1) {
		subscreen();
		cursor_set(MENU_BASE_ROW, 0);
		for (int i = 0; i < MenuOpt_Count; i++) {
			printf("   %s\n", menu_opt_str_list[i]);
		}

		cursor_set(gRow, gColumn);
		printf("->");
		switch (wait_for_pad()) {
			case BUTTON_DOWN:
				if ((gRow + 1) > OPT_MAX) break;
				gRow++;
				continue;
				break;
			case BUTTON_UP:
				if ((gRow - 1) < MENU_BASE_ROW) break;
				gRow--;
				continue;
				break;
			case BUTTON_A:
				handle_opt(row_to_opt(gRow));
				break;
			default:
				break;
			}
	}

}
