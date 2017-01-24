/*
 * arguments.c
 *
 * rmln - Only remove links.
 * Copyright (c) 2016 Ammon Smith
 *
 * rmln is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * rmln is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rmln.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <getopt.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "info.h"
#include "options.h"

struct options opt;

static void print_usage(FILE *stream)
{
	fprintf(stream,
		"Usage: %s [OPTION]... [FILE]...\n"
		"       %s [--help | --version]\n",
		opt.argv0, opt.argv0);
}

static void help_and_exit()
{
	print_usage(stdout);
	printf("\nOptions:\n"
	       "  -s, --symbolic-only       Only delete symbolic links.\n"
	       "  -l, --hard-only           Only delete hard links.\n"
	       "  -i, --interactive         Prompt before every removal.\n"
	       "  -f, --force               Ignore errors or missing files.\n"
	       "  -r, --recrusive           Recursively enter directories and delete any links inside.\n"
	       "  -v, --verbose             Print the name of every file being removed.\n"
	       "  -h, --help                Print this help message.\n"
	       "  -V, --version             Print the version and exit.\n\n");
	exit(0);
}

static void version_and_exit()
{
	printf("%s v%d.%d.%d [%s]\n"
	       "(%s %s) on %s.\n"
	       "Built %s, %s.\n"
	       "\n"
	       "%s is free software: you can redistribute it and/or modify\n"
	       "it under the terms of the GNU General Public License as published by\n"
	       "the Free Software Foundation, either version 2 of the License, or\n"
	       "(at your option) any later version.\n"
	       "\n"
	       "%s is distributed in the hope that it will be useful,\n"
	       "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	       "GNU General Public License for more details.\n",
	       PROGRAM_NAME,
	       PROGRAM_VERSION_MAJOR,
	       PROGRAM_VERSION_MINOR,
	       PROGRAM_VERSION_PATCH,
	       GIT_HASH,
	       COMPILER_NAME,
	       COMPILER_VERSION,
	       PLATFORM_NAME,
	       __DATE__,
	       __TIME__,
	       PROGRAM_NAME,
	       PROGRAM_NAME);
	puts("\nYou should have received a copy of the GNU General Public License\n"
	     "along with rmln.  If not, see <http://www.gnu.org/licenses/>.");
	exit(0);
}

int parse_args(int argc, char *argv[])
{
	int index, ch;
	const struct option long_options[] = {
		{ "symbolic",    no_argument, &opt.mode, DELETE_SYMBOLIC_LINKS },
		{ "hard-only",   no_argument, &opt.mode, DELETE_HARD_LINKS },
		{ "interactive", no_argument, NULL,      'i' },
		{ "force",       no_argument, NULL,      'f' },
		{ "recursive",   no_argument, NULL,      'r' },
		{ "verbose",     no_argument, NULL,      'v' },
		{ "help",        no_argument, NULL,      'h' },
		{ "version",     no_argument, NULL,      'V' },
		{ 0, 0, 0, 0 }
	};

	opt.mode = DELETE_ALL_LINKS;
	opt.interactive = 0;
	opt.recursive = 0;
	opt.verbose = 0;
	opt.force = 0;

	while ((ch = getopt_long(argc, argv, "slifrvhV", long_options, &index)) != -1) {
		switch (ch) {
		case 's':
			opt.mode = DELETE_SYMBOLIC_LINKS;
			break;
		case 'l':
			opt.mode = DELETE_HARD_LINKS;
			break;
		case 'i':
			opt.interactive = 1;
			break;
		case 'f':
			opt.force = 1;
			break;
		case 'r':
			opt.recursive = 1;
			break;
		case 'v':
			opt.verbose = 1;
			break;
		case 'h':
			help_and_exit();
			break;
		case 'V':
			version_and_exit();
			break;
		case '0':
			puts("_debug: got 0");
			break;
		case '?':
			exit(1);
		default:
			abort();
		}
	}
	if (optind == argc && !opt.force) {
		fprintf(stderr, "%s: missing operand\n", opt.argv0);
		exit(1);
	}
	return optind;
}
