/*
 * vector.h
 *
 * antz - My fictional assembly language and emulator.
 * Copyright (c) 2016 Ammon Smith
 *
 * antz is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * antz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with antz.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CHAR_VECTOR_H
#define __CHAR_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

struct char_vector_st {
    char *array;
    size_t length;
    size_t capacity;
};

typedef struct char_vector_st *char_vector;

char_vector char_vector_init();
char_vector char_vector_init_c(size_t capacity);
void char_vector_destroy(char_vector v);
void char_vector_clear(char_vector v);
bool char_vector_append(char_vector v, char item);
bool char_vector_prepend(char_vector v, char item);
bool char_vector_expand(char_vector v, size_t increase, char item);
bool char_vector_shrink(char_vector v, size_t decrease);
char char_vector_get(char_vector v, size_t index, bool *success);
bool char_vector_set(char_vector v, size_t index, char item);
bool char_vector_remove(char_vector v, size_t index);
char_vector char_vector_resize(char_vector v, size_t new_capacity);
char *char_vector_to_string(char_vector v);
char *char_vector_to_buffer(char_vector v);

#endif /* __CHAR_VECTOR_H */

