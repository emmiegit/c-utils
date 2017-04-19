/*
 * Makefile
 *
 * liblstring - Length-based string library
 * Copyright (c) 2017 Ammon Smith
 *
 * liblstring is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version
 * 2 of the License, or (at your option) any later version.
 *
 * liblstring is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with liblstring.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _LSTRING_H_
#define _LSTRING_H_

#include <stddef.h>

struct lstring {
	char *ptr;
	size_t len;
};

#endif /* _LSTRING_H_ */

