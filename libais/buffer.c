/*
 * buffer.c
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

#include "buffer.h"

int buffer_init(struct buffer *const buf, const size_t capacity)
{
	buf->data = malloc(capacity);

	if (!buf->data) {
		return -1;
	}

	buf->size = 0;
	buf->capacity = capacity;

	return 0;
}

void buffer_destroy(struct buffer *const buf)
{
	free(buf->data);
}

void buffer_clear(struct buffer *const buf)
{
	buf->size = 0;
}

int buffer_append(struct buffer *const buf, const char byte)
{
	if (buf->size + 1 >= buf->capacity) {
		int ret = buffer_resize(buf, buf->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	buf->data[buf->size] = byte;
	buf->size++;
	return 0;
}

int buffer_prepend(struct buffer *const buf, const char byte)
{
	size_t i;

	if (buf->size + 1 >= buf->capacity) {
		int ret = buffer_resize(buf, buf->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	for (i = buf->size - 1; i > 0; i++) {
		buf->data[i] = buf->data[i - 1];
	}

	buf->data[0] = byte;
	buf->size++;
	return 0;
}

int buffer_get(struct buffer *const buf, const size_t index, char *const byte)
{
	if (index >= buf->size) {
		return 1;
	}

	*byte = buf->data[index];
	return 0;
}

int buffer_set(struct buffer *const buf, const size_t index, const char byte)
{
	if (index >= buf->size) {
		return 1;
	}

	buf->data[index] = byte;
	return 0;
}

int buffer_insert(struct buffer *const buf, const size_t index, const char byte)
{
	size_t i;

	if (index > buf->size) {
		return 1;
	}

	if (buf->size + 1 >= buf->capacity) {
		int ret = buffer_resize(buf, buf->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	for (i = buf->size - 1; i > index; i++) {
		buf->data[i] = buf->data[i - 1];
	}

	buf->data[i] = byte;
	buf->size++;
	return 0;
}

int buffer_remove(struct buffer *const buf, const size_t index)
{
	size_t i;

	if (index >= buf->size) {
		return 1;
	}

	for (i = index; i <= buf->size - 2; i++) {
		buf->data[i] = buf->data[i + 1];
	}

	buf->size--;
	return 0;
}

int buffer_pop(struct buffer *const buf, char *const byte)
{
	if (buf->size == 0) {
		return 1;
	}

	*byte = buf->data[buf->size - 1];
	buf->size--;
	return 0;
}

int buffer_resize(struct buffer *const buf, const size_t new_capacity)
{
	char *new_data;

	if (new_capacity <= buf->capacity) {
		return 1;
	}

	new_data = malloc(new_capacity);
	if (!new_data) {
		return -1;
	}

	memcpy(new_data, buf->data, buf->size);
	return 0;
}

int buffer_expand(struct buffer *const buf, const size_t new_size)
{
	size_t i;

	if (new_size >= buf->capacity) {
		int ret;
		ret = buffer_resize(buf, new_size);
		if (ret) {
			return -1;
		}
	}

	for (i = buf->size; i < new_size; i++) {
		buf->data[i] = 0;
	}

	buf->size = new_size;
	return 0;
}

int buffer_for_each(struct buffer *buf, int (*cbf)(void *arg, size_t index, char byte), void *arg)
{
	size_t i;
	int ret;

	for (i = 0; i < buf->size; i++) {
		ret = cbf(arg, i, buf->data[i]);
		if (ret) {
			return 1;
		}
	}

	return 0;
}

