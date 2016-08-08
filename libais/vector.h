/*
 * vector.h
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

#ifndef __VECTOR_H
#define __VECTOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct vector {
		const void **items;
		size_t size;
		size_t capacity;
};

/*
 * These methods do not call malloc() or free()
 * on the vector struct. You are responsible for
 * getting and releasing the memory for it.
 */
int vector_init(struct vector *v, size_t capacity);
void vector_destory(struct vector *v);

void vector_clear(struct vector *v);
int vector_append(struct vector *v, void *item);
int vector_prepend(struct vector *v, void *item);
int vector_get(struct vector *v, size_t index, void **item);
int vector_set(struct vector *v, size_t index, const void *item);
int vector_insert(struct vector *v, size_t index, void *item);
int vector_remove(struct vector *v, size_t index);
int vector_resize(struct vector *v, size_t new_capacity);
int vector_expand(struct vector *v, size_t new_size);
int vector_pop(struct vector *v);

/*
 * Runs the callback function cbf on each item in the vector.
 * If cbf returns nonzero the loop aborts.
 */
int vector_for_each(struct vector *v,
				int (*cbf)(void *arg, size_t index, void *item), void *arg);

#define vector_get_size(v)				((v)->size)
#define vector_is_empty(v)				((v)->size == 0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VECTOR_H */

