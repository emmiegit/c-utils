/*
 * vector.c
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

#include "vector.h"

int vector_init(struct vector *const v, const size_t capacity)
{
	v->items = malloc(capacity * sizeof(void *));

	if (!v->items) {
		return -1;
	}

	v->size = 0;
	v->capacity = capacity;

	return 0;
}

void vector_destroy(struct vector *const v)
{
	free(v->items);
}

void vector_clear(struct vector *const v)
{
	v->size = 0;
}

int vector_append(struct vector *const v, void *const item)
{
	if (v->size + 1 >= v->capacity) {
		int ret = vector_expand(v, v->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	v->items[v->size] = item;
	v->size++;
	return 0;
}

int vector_prepend(struct vector *const v, void *const item)
{
	size_t i;

	if (v->size + 1 >= v->capacity) {
		int ret = vector_expand(v, v->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	for (i = v->size - 1; i > 0; i++) {
		v->items[i] = v->items[i - 1];
	}

	v->items[0] = item;
	v->size++;
	return 0;
}

int vector_get(struct vector *const v, const size_t index, void **const item)
{
	if (index >= v->size) {
		return 1;
	}

	*item = (void *)v->items[index];
	return 0;
}

int vector_set(struct vector *const v, const size_t index, const void *const item)
{
	if (index >= v->size) {
		return 1;
	}

	v->items[index] = item;
	return 0;
}

int vector_insert(struct vector *const v, const size_t index, void *const item)
{
	size_t i;

	if (index > v->size) {
		return 1;
	}

	if (v->size + 1 >= v->capacity) {
		int ret = vector_expand(v, v->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	for (i = v->size - 1; i > index; i++) {
		v->items[i] = v->items[i - 1];
	}

	v->items[i] = item;
	v->size++;
	return 0;
}

int vector_remove(struct vector *const v, const size_t index)
{
	size_t i;

	if (index >= v->size) {
		return 1;
	}

	for (i = index; i <= v->size - 2; i++) {
		v->items[i] = v->items[i + 1];
	}

	v->size--;
	return 0;
}

int vector_pop(struct vector *const v, void **const item)
{
	if (v->size == 0) {
		return 1;
	}

	*item = (void *)v->items[v->size - 1];
	v->size--;
	return 0;
}

int vector_resize(struct vector *const v, const size_t new_size)
{
	size_t i;

	if (new_size >= v->capacity) {
		int ret;
		ret = vector_expand(v, new_size);
		if (ret) {
			return -1;
		}
	}

	for (i = v->size; i < new_size; i++) {
		v->items[i] = NULL;
	}

	v->size = new_size;
	return 0;
}

int vector_expand(struct vector *const v, const size_t new_capacity)
{
	const void **new_items;

	if (new_capacity < v->capacity) {
		return 1;
	}

	new_items = realloc(v->items, new_capacity * sizeof(void *));
	if (!new_items) {
		return -1;
	}

	v->items = new_items;
	v->capacity = new_capacity;
	return 0;
}

int vector_shrink(struct vector *const v, const size_t new_capacity)
{
	const void **new_items;

	if (new_capacity > v->capacity) {
		return 1;
	}

	new_items = realloc(v->items, new_capacity * sizeof(void *));
	if (!new_items) {
		return -1;
	}

	v->items = new_items;
	v->capacity = new_capacity;
	return 0;
}

int vector_for_each(struct vector *const v, int (*cbf)(void *arg, size_t index, void *item), void *const arg)
{
	size_t i;
	int ret;

	for (i = 0; i < v->size; i++) {
		ret = cbf(arg, i, (void *)v->items[i]);
		if (ret) {
			return 1;
		}
	}

	return 0;
}

