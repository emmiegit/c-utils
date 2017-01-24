#include <stdio.h>
#include <curses.h>

int main()
{
	int counter;

	if (!initscr()) {
		perror("Unable to initalize curses");
		return 1;
	}

	raw();
	keypad(stdscr, TRUE);

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

		printw("\nID: %d\n", ch);
	}

	endwin();
	return 0;
}

