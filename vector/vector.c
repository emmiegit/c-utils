/*
 * vector.c
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
#include <stdlib.h>

#include "vector.h"

#define VECTOR_DEFAULT_CAPACITY 20

vector vector_init()
{
    return vector_init_c(VECTOR_DEFAULT_CAPACITY);
}

vector vector_init_c(size_t capacity)
{
    vector v = malloc(sizeof(struct vector_st));

    if (v == NULL) {
        return NULL;
    }

    v->array = malloc(capacity * sizeof(void *));
    v->capacity = capacity;
    v->length = 0;

    return v;
}

void vector_destroy(vector v)
{
    if (v) {
        free(v->array);
        free(v);
    }
}

void vector_clear(vector v)
{
    v->length = 0;
}

bool vector_append(vector v, void *item)
{
    if (v->length >= v->capacity) {
        vector_resize(v, v->capacity * 2);
    }

    v->array[v->length++] = item;
    return true;
}

bool vector_prepend(vector v, void *item)
{
    if (v->length >= v->capacity) {
        vector_resize(v, v->capacity * 2);
    }

    int i;
    for (i = v->length - 1; i >= 0; i--) {
        v->array[i + 1] = v->array[i];
    }

    v->array[0] = item;
    v->length++;
    return true;
}

bool vector_insert(vector v, size_t index, void *item)
{
    if (v->length >= v->capacity) {
        if (!vector_resize(v, v->capacity * 2)) {
            return false;
        }
    }

    size_t i;
    for (i = v->length - 1; i > index; i--) {
        v->array[i] = v->array[i + 1];
    }

    v->array[index] = item;
    v->length++;
    return true;
}

bool vector_expand(vector v, size_t increase, void *item)
{
    if (v->length + increase >= v->capacity) {
        vector_resize(v, v->length + increase + 4);
    }

    size_t i;
    for (i = 0; i < increase; i++) {
        v->array[v->length + i] = item;
    }

    v->length += increase;
    return true;
}

bool vector_shrink(vector v, size_t decrease)
{
    if (decrease > v->length) {
        return false;
    }

    v->length -= decrease;
    return true;
}

void *vector_get(vector v, size_t index, bool *success)
{
    if (index >= v->length) {
        if (success != NULL) {
            *success = false;
        }

        return NULL;
    }

    if (success != NULL) {
        *success = true;
    }

    return v->array[index];
}

bool vector_set(vector v, size_t index, void *item)
{
    if (index >= v->length) {
        return false;
    }

    v->array[index] = item;
    return true;
}

bool vector_remove(vector v, size_t index)
{
    size_t i;
    for (i = index; i < v->length; i++) {
        v->array[i] = v->array[i + 1];
    }

    v->length--;
    return true;
}

void vector_for_each(vector v, void (*f)(size_t, void *))
{
    size_t i;
    for (i = 0; i < v->length; i++) {
        f(i, v->array[i]);
    }
}

vector vector_resize(vector v, size_t new_capacity)
{
    if (v->length > new_capacity) {
        return NULL;
    }

    void **new_array = malloc(new_capacity * sizeof(void *));
    size_t i;
    for (i = 0; i < v->length; i++) {
        new_array[i] = v->array[i];
    }

    return v;
}

