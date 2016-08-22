/*
 * buffer.h
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

#ifndef __AIS_BUFFER_H
#define __AIS_BUFFER_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct buffer {
	char *data;
	size_t size;
	size_t capacity;
};

/*
 * These methods do not call malloc() or free()
 * on the buffer struct. You are responsible for
 * getting and releasing the memory for it.
 */
int buffer_init(struct buffer *buf, size_t capacity);
void buffer_destroy(struct buffer *buf);

void buffer_clear(struct buffer *buf);
int buffer_append(struct buffer *buf, char byte);
int buffer_prepend(struct buffer *buf, char byte);
int buffer_get(struct buffer *buf, size_t index, char *byte);
int buffer_set(struct buffer *buf, size_t index, char byte);
int buffer_insert(struct buffer *buf, size_t index, char byte);
int buffer_remove(struct buffer *buf, size_t index);
int buffer_pop(struct buffer *buf, char *byte);

/*
 * resize() changes the size (bigger or smaller)
 * expand() changes the capacity (only bigger)
 * shrink() changes the capacity (only smaller)
 */
int buffer_resize(struct buffer *buf, size_t new_size);
int buffer_expand(struct buffer *buf, size_t new_capacity);
int buffer_shrink(struct buffer *buf, size_t new_capacity);

/*
 * Runs the callback function cbf on each item in the buffer.
 * If cbf returns nonzero the loop aborts.
 */
int buffer_for_each(struct buffer *buf, int (*cbf)(void *arg, size_t index, char byte), void *arg);

int buffer_append_bytes(struct buffer *buf, const char *bytes, size_t length);
int buffer_prepend_bytes(struct buffer *buf, const char *bytes, size_t length);
int buffer_get_bytes(struct buffer *buf, size_t index, char *bytes, size_t length);
int buffer_set_bytes(struct buffer *buf, size_t index, const char *bytes, size_t length);
int buffer_insert_bytes(struct buffer *buf, size_t index, const char *bytes, size_t length);
int buffer_remove_bytes(struct buffer *buf, size_t index, size_t length);

#define buffer_append_int(buf, val)			\
	(buffer_append_bytes((buf), (void *)(&(val)), sizeof(val)))

#define buffer_prepend_int(buf, val)			\
	(buffer_prepend_bytes((buf), (void *)(&(val)), sizeof(val)))

#define buffer_get_int(buf, index, val)			\
	(buffer_get_bytes((buf), (index), (void *)(&(val)), sizeof(val)))

#define buffer_set_int(buf, index, val)			\
	(buffer_set_bytes((buf), (index), (void *)(&(val)), sizeof(val)))

#define buffer_insert_int(buf, index, val)		\
	(buffer_insert_bytes((buf), (index), (void *)(&(val)), sizeof(val)))

#define buffer_remove_int(buf, index, val)		\
	(buffer_remove_bytes((buf), (index), sizeof(val))

#define buffer_get_size(buf)				((buf)->size)
#define buffer_is_empty(buf)				((buf)->size == 0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AIS_BUFFER_H */

