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

#include "options.h"
#include "remove.h"

int main(int argc, char *argv[])
{
	int (*remove_func)(const char *path);
	int i, ret;

	opt.argv0 = argv[0];
	i = parse_args(argc, argv);
	remove_func = (opt.recursive) ? remove_recursive : remove_link;

	ret = 0;
	for (; i < argc; i++) {
		ret += (*remove_func)(argv[i]);
	}
	return (ret) ? EXIT_FAILURE : EXIT_SUCCESS;
}
