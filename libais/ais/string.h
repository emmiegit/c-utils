/*
 * string.h
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

#ifndef __AIS_STRING_H
#define __AIS_STRING_H

#include <stddef.h>

struct string {
	char *buffer;
	size_t length;
};

int string_init(struct string *str, size_t capacity);
void string_destroy(struct string *str);
int string_expand(struct string *str, size_t length);
int string_duplicate(struct string *dest_str, const struct string *src_str);
int string_set(struct string *str, const char *cptr);

#endif /* __AIS_STRING_H */

