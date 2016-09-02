/*
 * main.c
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

#include "arguments.h"
#include "remove.h"

/* Function implementations */
int main(int argc, const char *argv[])
{
	struct options opt;
	int (*remove_func)(const char *path, const struct options *opt);
	int i, ret;

	opt.program_name = argv[0];
	i = parse_arguments(&opt, argc, argv);
	remove_func = (opt.recursive) ? remove_recursive : remove_link;

	for (ret = 0; i < argc; i++) {
		ret += remove_func(argv[i], &opt);
	}

	return ret ? EXIT_FAILURE : EXIT_SUCCESS;
}

