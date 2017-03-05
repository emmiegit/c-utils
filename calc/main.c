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
	UNUSED(argv);

	if (argc != 1) {
		fprintf(stderr, "%s: warn: ignoring arguments\n",
			argv[0]);
	}

	print_version();
	execute_file();
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
