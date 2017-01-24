/*
 * options.h
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

#ifndef __OPTIONS_H
#define __OPTIONS_H

#define DELETE_SYMBOLIC_LINKS		0x1
#define DELETE_HARD_LINKS		0x2
#define DELETE_ALL_LINKS		(DELETE_SYMBOLIC_LINKS | DELETE_HARD_LINKS)

#define IF_DELETE_SYMLINKS(x)		((x) & DELETE_SYMBOLIC_LINKS)
#define IF_DELETE_HARD_LINKS(x)		((x) & DELETE_HARD_LINKS)

struct options {
	const char *argv0;
	int mode;

	unsigned interactive : 1;
	unsigned recursive   : 1;
	unsigned force       : 1;
	unsigned verbose     : 1;
};

extern struct options opt;

int parse_args(int argc, char *argv[]);

#endif /* __OPTIONS_H */

