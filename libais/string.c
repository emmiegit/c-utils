/*
 * string.c
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

#include <stdlib.h>
#include <string.h>

#include "ais/string.h"

int string_init(struct string *str, size_t capacity)
{
	str->buffer = malloc(capacity);
	str->length = capacity;
	if (!str->buffer) {
		return -1;
	}

	return 0;
}

void string_destroy(struct string *str)
{
	free(str->buffer);
}

int string_resize(struct string *str, size_t length)
{
	char *new_buf = realloc(str->buffer, length);
	if (!new_buf) {
		return -1;
	}

	if (length > str->length) {
		memset(new_buf + str->length, 0, length - str->length);
	}

	str->buffer = new_buf;
	str->length = length;
	return 0;
}

int string_set(struct string *str, const char *cptr)
{
	size_t len = strlen(cptr);
	memcpy(str->buffer, cptr, len);
	return 0;
}

