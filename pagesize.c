#define _XOPEN_SOURCE	500

#include <unistd.h>

#include <stdio.h>

#define UNUSED(x)	((void)(x))

int main(int argc, char **argv)
{
	const char *fmt_string;

	UNUSED(argc);
	UNUSED(argv);

	if (isatty(STDOUT_FILENO)) {
		fmt_string = "Page size: %ld bytes.\n";
	} else {
		fmt_string = "%ld";
	}

	printf(fmt_string, sysconf(_SC_PAGESIZE));
	return 0;
}
