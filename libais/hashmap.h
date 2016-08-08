/*
 * hashmap.h
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

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct hashmap_key {
	const char *buffer;
	size_t length;
};

struct hashmap_entry {
	struct hashmap_key key;
	const void *value;
	struct hashmap_entry *overflow;
};

struct hashmap {
	struct hashmap_entry *items;
	size_t size;
	size_t capacity;
};

/*
 * These methods do not call malloc() or free()
 * on the hashmap struct. You are responsible for
 * getting and releasing the memory for it.
 */
int hashmap_init(struct hashmap *map, size_t capacity);
void hashmap_destroy(struct hashmap *map);

void hashmap_clear(struct hashmap *map);
int hashmap_resize(struct hashmap *map, size_t new_capacity);

#define hashmap_get_size(map)		((map)->size)
#define hashmap_is_empty(map)		((map)->size == 0)

/*
 * insert - fails if the item exists
 * replace - fails if the item doesn't exist
 * update - will only fail due to an external error (e.g. out of memory)
 */
int hashmap_insert(struct hashmap *map, struct hashmap_key *key, const void *value);
int hashmap_replace(struct hashmap *map, struct hashmap_key *key, const void *value);
int hashmap_update(struct hashmap *map, struct hashmap_key *key, const void *value);

int hashmap_remove(struct hashmap *map, struct hashmap_key *key);
int hashmap_contains_key(struct hashmap *map, struct hashmap_key *key);
int hashmap_get(struct hashmap *map, struct hashmap_key *key, void **value);

/*
 * Returns a malloc-d array of size map->size. You are responsible for
 * free-ing it.
 */
struct hashmap_key *hashmap_list_keys(struct hashmap *map);
void **hashmap_list_values(struct hashmap *map);

/*
 * Runs the callback function cbf on each key/value pair in the map.
 * If cbf returns nonzero the loop aborts.
 */
int hashmap_for_each(struct hashmap *map,
	int (*cbf)(void *arg, struct hashmap_key *key, void *value), void *arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HASHMAP_H */

