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

int main(int argc, char *argv[])
{
	int i;

	printf("%s v%d.%d.%d\n",
		PROGRAM_NAME,
		PROGRAM_VERSION_MAJOR,
		PROGRAM_VERSION_MINOR,
		PROGRAM_VERSION_PATCH);
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
