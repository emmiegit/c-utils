/*
 * main.c
 *
 * calc - a simple CLI calculator
 * Copyright (c) 2017 Ammon Smith
 *
 * calc is available free of charge under the terms of the MIT
 * License. You are free to redistribute and/or modify it under those
 * terms. It is distributed in the hopes that it will be useful, but
 * WITHOUT ANY WARRANTY. See the LICENSE file for more details.
 *
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "calc.h"
#include "main.h"

int main(int argc, char *argv[])
{
	int i;

	print_version();

	if (argc == 1) {
		if (calc_file("<stdin>", stdin))
			return 1;
	} else for (i = 1; i < argc; i++) {
		FILE *fh;

		fh = fopen(argv[i], "r");
		if (!fh) {
			fprintf(stderr, "%s: %s: unable to open: %s\n",
				argv[0], argv[i], strerror(errno));
			return 1;
		}
		if (calc_file(argv[i], fh))
			return 1;
		if (fclose(fh))
			abort();
	}
	return 0;
}

void print_version(void)
{
	printf("%s v%d.%d.%d [%s]\n"
		"(%s %s) on %s.\n"
		"Built %s, %s.\n"
		"\n",
		PROGRAM_NAME,
		PROGRAM_VERSION_MAJOR,
		PROGRAM_VERSION_MINOR,
		PROGRAM_VERSION_PATCH,
		GIT_HASH,
		COMPILER_NAME,
		COMPILER_VERSION,
		PLATFORM_NAME,
		__DATE__,
		__TIME__);
}
