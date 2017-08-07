#include <stdio.h>
#include <curses.h>

int main()
{
	if (!initscr()) {
		fputs("Unable to initialize curses\n", stderr);
		return 1;
	}

	printw("ACS_BLOCK    "); addch(ACS_BLOCK);    printw("\n");
	printw("ACS_BOARD    "); addch(ACS_BOARD);    printw("\n");
	printw("ACS_BTEE     "); addch(ACS_BTEE);     printw("\n");
	printw("ACS_BULLET   "); addch(ACS_BULLET);   printw("\n");
	printw("ACS_CKBOARD  "); addch(ACS_CKBOARD);  printw("\n");
	printw("ACS_DARROW   "); addch(ACS_DARROW);   printw("\n");
	printw("ACS_DEGREE   "); addch(ACS_DEGREE);   printw("\n");
	printw("ACS_DIAMOND  "); addch(ACS_DIAMOND);  printw("\n");
	printw("ACS_GEQUAL   "); addch(ACS_GEQUAL);   printw("\n");
	printw("ACS_HLINE    "); addch(ACS_HLINE);    printw("\n");
	printw("ACS_LANTERN  "); addch(ACS_LANTERN);  printw("\n");
	printw("ACS_LARROW   "); addch(ACS_LARROW);   printw("\n");
	printw("ACS_LEQUAL   "); addch(ACS_LEQUAL);   printw("\n");
	printw("ACS_LLCORNER "); addch(ACS_LLCORNER); printw("\n");
	printw("ACS_LRCORNER "); addch(ACS_LRCORNER); printw("\n");
	printw("ACS_LTEE     "); addch(ACS_LTEE);     printw("\n");
	printw("ACS_NEQUAL   "); addch(ACS_NEQUAL);   printw("\n");
	printw("ACS_PI       "); addch(ACS_PI);       printw("\n");
	printw("ACS_PLMINUS  "); addch(ACS_PLMINUS);  printw("\n");
	printw("ACS_PLUS     "); addch(ACS_PLUS);     printw("\n");
	printw("ACS_RARROW   "); addch(ACS_RARROW);   printw("\n");
	printw("ACS_RTEE     "); addch(ACS_RTEE);     printw("\n");
	printw("ACS_S1       "); addch(ACS_S1);       printw("\n");
	printw("ACS_S3       "); addch(ACS_S3);       printw("\n");
	printw("ACS_S7       "); addch(ACS_S7);       printw("\n");
	printw("ACS_S9       "); addch(ACS_S9);       printw("\n");
	printw("ACS_STERLING "); addch(ACS_STERLING); printw("\n");
	printw("ACS_TTEE     "); addch(ACS_TTEE);     printw("\n");
	printw("ACS_UARROW   "); addch(ACS_UARROW);   printw("\n");
	printw("ACS_ULCORNER "); addch(ACS_ULCORNER); printw("\n");
	printw("ACS_URCORNER "); addch(ACS_URCORNER); printw("\n");
	printw("ACS_VLINE    "); addch(ACS_VLINE);    printw("\n");

	getch();
	endwin();
	return 0;
}
