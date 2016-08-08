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

#define MAX(x, y)			(((x) > (y)) ? (x) : (y))
#define MIN(x, y)			(((x) < (y)) ? (x) : (y))

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
		int ret = buffer_expand(buf, buf->capacity * 2);
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
	if (buf->size + 1 >= buf->capacity) {
		int ret = buffer_expand(buf, buf->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	memmove(buf->data + 1, buf->data, buf->size);
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
	if (index > buf->size) {
		return 1;
	}

	if (buf->size + 1 >= buf->capacity) {
		int ret = buffer_expand(buf, buf->capacity * 2);
		if (ret) {
			return -1;
		}
	}

	memmove(buf->data + index, buf->data + index + 1, buf->size - index);
	buf->data[index] = byte;
	buf->size++;
	return 0;
}

int buffer_remove(struct buffer *const buf, const size_t index)
{
	if (index >= buf->size) {
		return 1;
	}

	memmove(buf->data + index + 1, buf->data + index, buf->size - index - 1);
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

int buffer_resize(struct buffer *const buf, const size_t new_size)
{
	if (new_size >= buf->capacity) {
		int ret;
		ret = buffer_expand(buf, new_size);
		if (ret) {
			return -1;
		}
	}

	memset(buf->data + buf->size, 0, new_size);
	buf->size = new_size;
	return 0;
}

int buffer_expand(struct buffer *const buf, const size_t new_capacity)
{
	char *new_data;

	if (new_capacity < buf->capacity) {
		return 1;
	}

	new_data = realloc(buf->data, new_capacity);
	if (!new_data) {
		return -1;
	}

	buf->capacity = new_capacity;
	return 0;
}

int buffer_shrink(struct buffer *const buf, const size_t new_capacity)
{
	char *new_data;

	if (new_capacity > buf->capacity) {
		return 1;
	}

	new_data = realloc(buf->data, new_capacity);
	if (!new_data) {
		return -1;
	}

	buf->capacity = new_capacity;
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

int buffer_append_bytes(struct buffer *buf, const char *bytes, size_t length)
{
	if (buf->size + length >= buf->capacity) {
		int ret;
		ret = buffer_expand(buf, MAX(buf->size + length + 1, buf->capacity * 2));
		if (ret) {
			return -1;
		}
	}

	memcpy(buf->data + buf->size, bytes, length);
	buf->size += length;
	return 0;
}

int buffer_prepend_bytes(struct buffer *buf, const char *bytes, size_t length)
{
	if (buf->size + length >= buf->capacity) {
		int ret;
		ret = buffer_expand(buf, MAX(buf->size + length + 1, buf->capacity * 2));
		if (ret) {
			return -1;
		}
	}

	memmove(buf->data + length, buf->data, length);
	memcpy(buf->data, bytes, length);
	buf->size += length;
	return 0;
}

int buffer_get_bytes(struct buffer *buf, size_t index, char *bytes, size_t length)
{
	if (index + length >= buf->size) {
		return 1;
	}

	memcpy(bytes, buf->data + index, length);
	return 0;
}

int buffer_set_bytes(struct buffer *buf, size_t index, const char *bytes, size_t length)
{
	if (index + length >= buf->size) {
		return 1;
	}

	memcpy(buf->data + index, bytes, length);
	return 0;
}

int buffer_insert_bytes(struct buffer *buf, size_t index, const char *bytes, size_t length)
{
	if (buf->size + length >= buf->capacity) {
		int ret;
		ret = buffer_expand(buf, MAX(buf->size + length + 1, buf->capacity * 2));
		if (ret) {
			return -1;
		}
	}

	memmove(buf->data + index + length, buf->data + index, length);
	memcpy(buf->data + index, bytes, length);
	buf->size += length;
	return 0;
}

int buffer_remove_bytes(struct buffer *buf, size_t index, size_t length)
{
	if (index + length >= buf->size) {
		return 1;
	}

	memmove(buf->data + index, buf->data + index + length, MIN(length, buf->size - index));
	buf->size -= length;
	return 0;
}

