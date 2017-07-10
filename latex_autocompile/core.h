/*
 * core.h
 *
 * latex-autocompile - Automatically recompile LaTeX sources and
 *                     refresh the MuPDF viewer
 * Copyright (c) 2016-2017 Ammon Smith
 *
 * latex-autocompile is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * latex-autocompile is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with latex-autocompile.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _CORE_H_
#define _CORE_H_

#include <stddef.h>

#include "config.h"

/* Macros */

#define UNUSED(x)		((void)(x))
#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))
#define STATIC_ASSERT(x)	((void)sizeof(char[2 * (!!(x)) - 1]))

#define DEFAULT(x,a)		((x) ? (x) : (a))
#define MIN(x,y)		(((x) < (y)) ? (x) : (y))
#define MAX(x,y)		(((x) > (y)) ? (x) : (y))

#if USE_VFORK
# define FORK			vfork
#else
# define FORK			fork
#endif /* USE_VFORK */

/* Functions */

void die(const char *fmt, ...);

#endif /* _CORE_H_ */

