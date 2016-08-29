#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <libgen.h>
#include <termios.h>
#include <unistd.h>

static void init_raw(bool init);
static bool get_character(bool print_prompt);

int main(int argc, char *argv[])
{
	/* Variable declarations */
	bool keep_running = false;
	bool print_prompt;
	int i;

	/* Parse arguments */
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-a")
			|| !strcmp(argv[i], "--all")) {
			keep_running = true;
		} else if (!strcmp(argv[i], "--help")) {
			printf("Usage: %s [-a]\n"
			       " -a, --all               Read all characters until EOF.\n",
			       basename(argv[0]));
			return EXIT_SUCCESS;
		} else {
			fprintf(stderr,
				"Unknown option: %s\n"
				"Usage: %s [-a]\n",
				argv[i],
				basename(argv[0]));
			return EXIT_FAILURE;
		}
	}

	/* Initialize raw mode */
	init_raw(true);

	/* Determine runtime settings */
	print_prompt = isatty(STDIN_FILENO);

	/* Read characters */
	do {
		keep_running &= get_character(print_prompt);
	} while(keep_running);

	/* Deinitialize raw mode */
	init_raw(false);

	/* Exit */
	return EXIT_SUCCESS;
}

static void init_raw(bool init)
{
	static struct termios oldterm, newterm;

	if (init) {
		tcgetattr(STDIN_FILENO, &oldterm);
		newterm = oldterm;
		newterm.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newterm);
	} else {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
	}
}

static bool get_character(bool print_prompt)
{
	/* Variable declarations */
	int ch;

	/* Get character */
	if (print_prompt) {
		printf("Enter a character: ");
		ch = getchar();
		putchar('\n');
	} else {
		ch = getchar();
	}

	/* Print character */
	if (ch == EOF) {
		printf("<EOF>\n\n");
		return false;
	} else {
		printf("Character: '%c'\n", ch);
		printf("Hex: %02x\n", ch);
		printf("Decimal: %d\n", ch);
		printf("Octal: %03o\n\n", ch);
		return true;
	}
}

