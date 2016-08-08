/*
 * hashset.h
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

#ifndef __HASHSET_H
#define __HASHSET_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct hashset_item {
	const char *buffer;
	size_t length;
};

struct hashset_entry {
	struct hashset_item item;
	struct hashset_entry *overflow;
};

struct hashset {
	struct hashset_entry *items;
	size_t size;
	size_t capacity;
};

/*
 * These methods do not call malloc() or free()
 * on the hashset struct. You are responsible for
 * getting and releasing the memory for it.
 */
int hashset_init(struct hashset *set, size_t capacity);
void hashset_destroy(struct hashset *set);

void hashset_clear(struct hashset *set);
int hashset_resize(struct hashset *set, size_t new_capacity);

#define hashset_get_size(set)		((set)->size)
#define hashset_is_empty(set)		((set)->size == 0)

/*
 * insert - fails if the item exists
 * update - reports success even if item exists
 */
int hashset_insert(struct hashset *set, struct hashset_item *item);
int hashset_update(struct hashset *set, struct hashset_item *item);

int hashset_remove(struct hashset *set, struct hashset_item *item);
int hashset_contains(struct hashset *set, struct hashset_item *item);

/*
 * Returns a malloc-d array of size set->size. You are responsible for
 * free-ing it.
 */
struct hashset_item *hashset_list_items(struct hashset *set);

/*
 * Runs the callback function cbf on each item pair in the set.
 * If cbf returns nonzero the loop aborts.
 */
int hashset_for_each(struct hashset *set,
	int (*cbf)(void *arg, struct hashset_item *item), void *arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HASHSET_H */

