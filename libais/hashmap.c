/*
 * hashmap.c
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
#include "util.h"

#define GET_INDEX(key, map)		(djb2_hash((key)->buffer, (key)->length) % (map)->capacity)
#define ENTRY_IS_EMPTY(x)		(!(x)->key.buffer)
#define KEYS_ARE_EQUAL(x, y)		\
	(((x) == (y)) || (((x)->length == (y)->length) && (!memcmp((x), (y), (x)->length))))

int hashmap_init(struct hashmap *const map, const size_t capacity)
{
	size_t i;

	if (capacity == 0) {
		return -1;
	}

	map->size = 0;
	map->capacity = capacity;
	map->items = malloc(capacity * sizeof(struct hashmap_entry));

	if (!map->items) {
		return -1;
	}

	for (i = 0; i < capacity; i++) {
		map->items[i].key.buffer = NULL;
		map->items[i].overflow = NULL;
	}

	return 0;
}

static void hashmap_entry_destroy(struct hashmap_entry *const entry)
{
	if (!entry) {
		return;
	}

	if (entry->overflow) {
		hashmap_entry_destroy(entry->overflow);
	}

	free(entry);
}

void hashmap_destroy(struct hashmap *const map)
{
	size_t i;

	if (!map) {
		return;
	}

	if (!map->items) {
		return;
	}

	for (i = 0; i < map->capacity; i++) {
		hashmap_entry_destroy(map->items[i].overflow);
	}

	free(map->items);
}

void hashmap_clear(struct hashmap *const map)
{
	size_t i;

	for (i = 0; i < map->capacity; i++) {
		map->items[i].key.buffer = NULL;
		hashmap_entry_destroy(map->items[i].overflow);
	}

	map->size = 0;
}

int hashmap_insert(struct hashmap *const map, struct hashmap_key *const key, const void *const value)
{
	size_t index;
	struct hashmap_entry *entry;

	index = GET_INDEX(key, map);
	entry = &map->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		map->size++;
		entry->key = *key;
		return 0;
	} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
		return 1;
	} else do {
		if (!entry->overflow) {
			entry->overflow = malloc(sizeof(struct hashmap_entry));

			if (!entry->overflow) {
				return -1;
			}

			map->size++;
			entry->overflow->key = *key;
			entry->overflow->value = value;
			entry->overflow->overflow = NULL;
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			map->size++;
			entry->overflow->key = *key;
			entry->overflow->value = value;
			return 0;
		} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
			return 1;
		}

		entry = entry->overflow;
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashmap_replace(struct hashmap *const map, struct hashmap_key *const key, const void *const value)
{
	size_t index;
	struct hashmap_entry *entry;

	index = GET_INDEX(key, map);
	entry = &map->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		return 1;
	} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
		entry->value = value;
		return 0;
	} else do {
		if (!entry->overflow) {
			return 1;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			return 1;
		} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
			entry->value = value;
		}

		entry = entry->overflow;
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashmap_update(struct hashmap *const map, struct hashmap_key *const key, const void *const value)
{
	size_t index;
	struct hashmap_entry *entry;

	index = GET_INDEX(key, map);
	entry = &map->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		map->size++;
		entry->key = *key;
		return 0;
	} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
		entry->value = value;
		return 0;
	} else do {
		if (!entry->overflow) {
			entry->overflow = malloc(sizeof(struct hashmap_entry));

			if (!entry->overflow) {
				return -1;
			}

			map->size++;
			entry->overflow->key = *key;
			entry->overflow->value = value;
			entry->overflow->overflow = NULL;
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			map->size++;
			entry->overflow->key = *key;
			entry->overflow->value = value;
			return 0;
		} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
			entry->value = value;
			return 0;
		}

		entry = entry->overflow;
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashmap_remove(struct hashmap *const map, struct hashmap_key *const key)
{
	size_t index;
	struct hashmap_entry *entry;

	index = GET_INDEX(key, map);
	entry = &map->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		return 1;
	} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
		if (entry->overflow) {
			entry->key = entry->overflow->key;
			entry->value = entry->overflow->value;
			entry->overflow = entry->overflow->overflow;
		} else {
			entry->key.buffer = NULL;
		}

		map->size--;
		return 0;
	} else do {
		if (!entry->overflow) {
			return 1;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			return 1;
		} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
			if (entry->overflow) {
				entry->key = entry->overflow->key;
				entry->value = entry->overflow->value;
				entry->overflow = entry->overflow->overflow;
			} else {
				entry->key.buffer = NULL;
			}

			map->size--;
			return 0;
		}
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashmap_contains_key(struct hashmap *const map, struct hashmap_key *const key)
{
	size_t index;
	struct hashmap_entry *entry;

	index = GET_INDEX(key, map);
	entry = &map->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		return 0;
	} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
		return 1;
	} else do {
		if (!entry->overflow) {
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			return 0;
		} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
			return 1;
		}
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashmap_get(struct hashmap *const map, struct hashmap_key *const key, void **const value)
{
	size_t index;
	struct hashmap_entry *entry;

	index = GET_INDEX(key, map);
	entry = &map->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		return 0;
	} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
		*value = (void *)entry->value;
		return 1;
	} else do {
		if (!entry->overflow) {
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			return 0;
		} else if (KEYS_ARE_EQUAL(key, &entry->key)) {
			*value = (void *)entry->value;
			return 1;
		}
	} while (entry->overflow);

	assert(0);
	return -1;
}

struct hashmap_key *hashmap_list_keys(struct hashmap *const map)
{
	struct hashmap_key *keys;
	struct hashmap_entry *entry;
	size_t i, j;

	keys = malloc(map->size * sizeof(struct hashmap_key));
	if (!keys) {
		return NULL;
	}

	for (i = 0, j = 0; i < map->capacity; i++) {
		entry = &map->items[i];

		if (!ENTRY_IS_EMPTY(entry)) {
			assert(j < map->size);
			keys[j++] = entry->key;

			while (entry->overflow && !ENTRY_IS_EMPTY(entry->overflow)) {
				assert(j < map->size);
				keys[j++] = entry->overflow->key;
				entry = entry->overflow;
			}
		}
	}

	return keys;
}

void **hashmap_list_values(struct hashmap *const map)
{
	void **values;
	struct hashmap_entry *entry;
	size_t i, j;

	values = malloc(map->size * sizeof(struct hashmap_key));
	if (!values) {
		return NULL;
	}

	for (i = 0, j = 0; i < map->capacity; i++) {
		entry = &map->items[i];

		if (!ENTRY_IS_EMPTY(entry)) {
			assert(j < map->size);
			values[j++] = (void *)entry->value;

			while (entry->overflow && !ENTRY_IS_EMPTY(entry->overflow)) {
				assert(j < map->size);
				values[j++] = (void *)entry->overflow->value;
				entry = entry->overflow;
			}
		}
	}

	return values;
}

int hashmap_for_each(struct hashmap *const map,
	int (*cbf)(void *arg, struct hashmap_key *key, void *value), void *arg)
{
	struct hashmap_entry *entry;
	size_t i;
	int ret;

	for (i = 0; i < map->capacity; i++) {
		entry = &map->items[i];

		if (!ENTRY_IS_EMPTY(entry)) {
			ret = cbf(arg, &entry->key, (void *)entry->value);
			if (ret) {
				return 1;
			}

			while (entry->overflow && !ENTRY_IS_EMPTY(entry->overflow)) {
				ret = cbf(arg, &entry->key, (void *)entry->value);
				if (ret) {
					return 1;
				}

				entry = entry->overflow;
			}
		}
	}

	return 0;
}

