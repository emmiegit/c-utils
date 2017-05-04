/*
 * util.h
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

#ifndef _UTIL_H_
#define _UTIL_H_

#include "core.h"

/* String functions */

int endswith(const char *str, const char *suffix, size_t suflen);
char *replace_ext(const char *str, const char *ext, size_t extlen);

/* Process functions */

int compile_command(const char *filename);

#endif /* _UTIL_H_ */

