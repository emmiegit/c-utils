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

#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdbool.h>
#include <stddef.h>

struct vector_st {
    void **array;
    size_t length;
    size_t capacity;
};

typedef struct vector_st *vector;

vector vector_init();
vector vector_init_c(size_t capacity);
void vector_destroy(vector v);
void vector_clear(vector v);
bool vector_append(vector v, void *item);
bool vector_prepend(vector v, void *item);
bool vector_insert(vector v, size_t index, void *item);
bool vector_expand(vector v, size_t increase, void *item);
bool vector_shrink(vector v, size_t decrease);
void *vector_get(vector v, size_t index, bool *success);
bool vector_set(vector v, size_t index, void *item);
bool vector_remove(vector v, size_t index);
vector vector_resize(vector v, size_t new_capacity);

#endif /* __VECTOR_H */

