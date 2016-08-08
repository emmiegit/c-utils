/*
 * util.c
 *
 * libais - Ammon's C library
 * Copyright (c) 2016 Ammon Smith
 *
 * libais is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version
 * 2 of the License, or (at your option) any later version.
 *
 * libais is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with libais.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "util.h"

unsigned long djb2_hash(const char *buffer, size_t length)
{
	size_t i;
	unsigned long hash = 5381;

	for (i = 0; i < length; i++) {
		/* hash * 33 + ch */
		hash = ((hash << 5) + hash) + buffer[i];
	}

	return hash;
}

