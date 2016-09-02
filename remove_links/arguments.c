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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "arguments.h"
#include "info.h"

/* Static function declarations */
static void set_defaults(struct options *opt);
static void parse_short_arguments(struct options *opt, const char *flags);
static void print_usage(FILE *stream, const char *program_name);
static void help_and_exit(const char *program_name);
static void version_and_exit();

/* Function implementations */
static void set_defaults(struct options *opt)
{
	opt->mode = DELETE_ALL_LINKS;
	opt->interactive = false;
	opt->recursive = false;
	opt->verbose = false;
}

int parse_arguments(struct options *opt, int argc, const char *argv[])
{
	int i;

	set_defaults(opt);

	if (argc == 1) {
		print_usage(stderr, opt->program_name);
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--")) {
			i++;
			break;
		} else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--symbolic-only")) {
			if (opt->mode == DELETE_HARD_LINKS) {
				fprintf(stderr, "%s: conflicting options: '-s' and '-l'\n",
					opt->program_name);
				print_usage(stderr, opt->program_name);
				exit(EXIT_FAILURE);
			}

			opt->mode = DELETE_SYMBOLIC_LINKS;
		} else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--hard-only")) {
			if (opt->mode == DELETE_SYMBOLIC_LINKS) {
				fprintf(stderr, "%s: conflicting options: '-s' and '-l'\n",
					opt->program_name);
				print_usage(stderr, opt->program_name);
				exit(EXIT_FAILURE);
			}

			opt->mode = DELETE_HARD_LINKS;
		} else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--interactive")) {
			opt->interactive = true;
		} else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--recursive")) {
			opt->recursive = true;
		} else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
			opt->verbose = true;
		} else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			help_and_exit(opt->program_name);
		} else if (!strcmp(argv[i], "-V") || !strcmp(argv[i], "--version")) {
			version_and_exit();
		} else if (argv[i][0] == '-') {
			if (argv[i][1] == '-') {
				fprintf(stderr, "%s: invalid flag: %s\n",
					opt->program_name, argv[i]);
				print_usage(stderr, opt->program_name);
				exit(EXIT_FAILURE);
			}

			parse_short_arguments(opt, argv[i] + 1);
		} else {
			/* It's a file, not an option */
			break;
		}
	}

	/* Check if any files were passed */
	if (i == argc) {
		fprintf(stderr, "%s: missing operand\n", opt->program_name);
		print_usage(stderr, opt->program_name);
		exit(EXIT_FAILURE);
	}

	return i;
}

static void parse_short_arguments(struct options *const opt, const char *const flags)
{
	int i;
	for (i = 0; flags[i]; i++) {
		switch (flags[i]) {
		case 's':
			if (opt->mode == DELETE_HARD_LINKS) {
				fprintf(stderr, "%s: conflicting options: '-s' and '-l'\n",
					opt->program_name);
				print_usage(stderr, opt->program_name);
				exit(EXIT_FAILURE);
			}

			opt->mode = DELETE_SYMBOLIC_LINKS;
			break;
		case 'l':
			if (opt->mode == DELETE_SYMBOLIC_LINKS) {
				fprintf(stderr, "%s: conflicting options: 's' and '-l'\n",
					opt->program_name);
				print_usage(stderr, opt->program_name);
				exit(EXIT_FAILURE);
			}

			opt->mode = DELETE_HARD_LINKS;
			break;
		case 'i':
			opt->interactive = true;
			break;
		case 'r':
			opt->recursive = true;
			break;
		case 'v':
			opt->verbose = true;
			break;
		case 'h':
			help_and_exit(opt->program_name);
			break;
		case 'V':
			version_and_exit();
			break;
		default:
			fprintf(stderr, "%s: invalid flag: '-%c'\n",
				opt->program_name, flags[i]);
			print_usage(stderr, opt->program_name);
		}
	}
}

static void print_usage(FILE *const stream, const char *const program_name)
{
	fprintf(stream,
		"Usage: %s [OPTION]... [FILE]...\n"
		"       %s [--help | --version]\n",
		program_name, program_name);
}

static void help_and_exit(const char *const program_name)
{
	print_usage(stdout, program_name);

	printf("\nOptions:\n"
	       "  -s, --symbolic-only       Only delete symbolic links.\n"
	       "  -l, --hard-only           Only delete hard links.\n"
	       "  -i, --interactive         Prompt before every removal.\n"
	       "  -r, --recrusive           Recursively enter directories and delete any links inside.\n"
	       "  -v, --verbose             Print the name of every file being removed.\n"
	       "  -h, --help                Print this help message.\n"
	       "  -V, --version             Print the version and exit.\n\n");

	exit(EXIT_SUCCESS);
}

static void version_and_exit()
{
	printf("%s %s [%s]\n"
	       "Copyright (c) %s %s\n"
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
	       PROGRAM_NAME, PROGRAM_VERSION, GIT_HASH, PROGRAM_YEARS, PROGRAM_AUTHORS,
	       PROGRAM_NAME, PROGRAM_NAME);

	printf("\nYou should have received a copy of the GNU General Public License\n"
	       "along with rmln.  If not, see <http://www.gnu.org/licenses/>.\n");

	exit(EXIT_SUCCESS);
}

