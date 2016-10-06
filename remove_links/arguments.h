/*
 * arguments.h
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

#ifndef __ARGUMENTS_H
#define __ARGUMENTS_H

#undef EXTERN
#ifndef __ARGUMENTS_EXTERN
# define __ARGUMENTS_EXTERN
# define EXTERN extern
#else
# define EXTERN
#endif /* __ARGUMENTS_EXTERN */

#include "options.h"

EXTERN struct options opt;

int parse_arguments(int argc, const char *argv[]);

#endif /* __ARGUMENTS_H */

