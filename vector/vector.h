/*
 * vector.h
 *
 * libaisvector
 * Copyright (c) 2016 Ammon Smith
 *
 * This file is available to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
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
void vector_for_each(vector v, void (*f)(size_t, void *));
vector vector_resize(vector v, size_t new_capacity);

#define vector_is_empty(v)      ((v)->size == 0)

#endif /* __VECTOR_H */

