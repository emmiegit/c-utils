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

#ifndef __BUFFER_H
#define __BUFFER_H

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

/* TODO add methods for int16_t, int32_t and int64_t */
void buffer_clear(struct buffer *buf);
int buffer_append(struct buffer *buf, char byte);
int buffer_prepend(struct buffer *buf, char byte);
int buffer_get(struct buffer *buf, size_t index, char *byte);
int buffer_set(struct buffer *buf, size_t index, char byte);
int buffer_insert(struct buffer *buf, size_t index, char byte);
int buffer_remove(struct buffer *buf, size_t index);
int buffer_pop(struct buffer *buf, char *byte);
int buffer_resize(struct buffer *buf, size_t new_capacity);
int buffer_expand(struct buffer *buf, size_t new_size);

/*
 * Runs the callback function cbf on each item in the buffer.
 * If cbf returns nonzero the loop aborts.
 */
int buffer_for_each(struct buffer *buf, int (*cbf)(void *arg, size_t index, char byte), void *arg);

#define buffer_get_size(buf)				((buf)->size)
#define buffer_is_empty(buf)				((buf)->size == 0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __BUFFER_H */

