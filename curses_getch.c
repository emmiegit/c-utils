#include <stdio.h>
#include <curses.h>

int main()
{
	int counter;

	if (!initscr()) {
		fputs("Unable to initalize curses\n", stderr);
		return 1;
	}

	raw();
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS, NULL);

	printw("Start entering characters:\n(Hit 'q' to quit.)\n");

	for (counter = 0; 1; counter++) {
		int ch;

		if (counter >= 25) {
			erase();
			move(0, 0);
			refresh();
			counter = 0;
		}

		printw("> ");
		ch = getch();
		if (ch == 'q') {
			endwin();
			return 0;
		}

		printw("\nKey: %s\nID: %d\n", keyname(ch), ch);
	}

	endwin();
	return 0;
}

