/*
 * hashset.c
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

#include "ais/hashset.h"
#include "ais/util.h"

#define GET_INDEX(item, set)		(djb2_hash((item)->buffer, (item)->length) % (set)->capacity)
#define ENTRY_IS_EMPTY(x)		(!(x)->item.buffer)
#define ITEMS_ARE_EQUAL(x, y)		\
	(((x) == (y)) || (((x)->length == (y)->length) && (!memcmp((x), (y), (x)->length))))

int hashset_init(struct hashset *const set, const size_t capacity)
{
	size_t i;

	if (capacity == 0) {
		return -1;
	}

	set->size = 0;
	set->capacity = capacity;
	set->items = malloc(capacity * sizeof(struct hashset_entry));

	if (!set->items) {
		return -1;
	}

	for (i = 0; i < capacity; i++) {
		set->items[i].item.buffer = NULL;
		set->items[i].overflow = NULL;
	}

	return 0;
}

static void hashset_entry_destroy(struct hashset_entry *const entry)
{
	if (!entry) {
		return;
	}

	if (entry->overflow) {
		hashset_entry_destroy(entry->overflow);
	}

	free(entry);
}

void hashset_destroy(struct hashset *const set)
{
	size_t i;

	if (!set) {
		return;
	}

	if (!set->items) {
		return;
	}

	for (i = 0; i < set->capacity; i++) {
		hashset_entry_destroy(set->items[i].overflow);
	}

	free(set->items);
}

void hashset_clear(struct hashset *const set)
{
	size_t i;

	for (i = 0; i < set->capacity; i++) {
		set->items[i].item.buffer = NULL;
		hashset_entry_destroy(set->items[i].overflow);
	}

	set->size = 0;
}

int hashset_insert(struct hashset *const set, struct hashset_item *const item)
{
	size_t index;
	struct hashset_entry *entry;

	index = GET_INDEX(item, set);
	entry = &set->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		set->size++;
		entry->item = *item;
		return 0;
	} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
		return 1;
	} else do {
		if (!entry->overflow) {
			entry->overflow = malloc(sizeof(struct hashset_entry));

			if (!entry->overflow) {
				return -1;
			}

			set->size++;
			entry->overflow->item = *item;
			entry->overflow->overflow = NULL;
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			set->size++;
			entry->overflow->item = *item;
			return 0;
		} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
			return 1;
		}

		entry = entry->overflow;
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashset_update(struct hashset *const set, struct hashset_item *const item)
{
	size_t index;
	struct hashset_entry *entry;

	index = GET_INDEX(item, set);
	entry = &set->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		set->size++;
		entry->item = *item;
		return 0;
	} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
		return 0;
	} else do {
		if (!entry->overflow) {
			entry->overflow = malloc(sizeof(struct hashset_entry));

			if (!entry->overflow) {
				return -1;
			}

			set->size++;
			entry->overflow->item = *item;
			entry->overflow->overflow = NULL;
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			set->size++;
			entry->overflow->item = *item;
			return 0;
		} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
			return 0;
		}

		entry = entry->overflow;
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashset_remove(struct hashset *const set, struct hashset_item *const item)
{
	size_t index;
	struct hashset_entry *entry;

	index = GET_INDEX(item, set);
	entry = &set->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		return 1;
	} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
		if (entry->overflow) {
			entry->item = entry->overflow->item;
			entry->overflow = entry->overflow->overflow;
		} else {
			entry->item.buffer = NULL;
		}

		set->size--;
		return 0;
	} else do {
		if (!entry->overflow) {
			return 1;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			return 1;
		} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
			if (entry->overflow) {
				entry->item = entry->overflow->item;
				entry->overflow = entry->overflow->overflow;
			} else {
				entry->item.buffer = NULL;
			}

			set->size--;
			return 0;
		}
	} while (entry->overflow);

	assert(0);
	return -1;
}

int hashset_contains(struct hashset *const set, struct hashset_item *const item)
{
	size_t index;
	struct hashset_entry *entry;

	index = GET_INDEX(item, set);
	entry = &set->items[index];

	if (ENTRY_IS_EMPTY(entry)) {
		return 0;
	} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
		return 1;
	} else do {
		if (!entry->overflow) {
			return 0;
		} else if (ENTRY_IS_EMPTY(entry->overflow)) {
			return 0;
		} else if (ITEMS_ARE_EQUAL(item, &entry->item)) {
			return 1;
		}
	} while (entry->overflow);

	assert(0);
	return -1;
}

struct hashset_item *hashset_list_items(struct hashset *const set)
{
	struct hashset_item *items;
	struct hashset_entry *entry;
	size_t i, j;

	items = malloc(set->size * sizeof(struct hashset_item));
	if (!items) {
		return NULL;
	}

	for (i = 0, j = 0; i < set->capacity; i++) {
		entry = &set->items[i];

		if (!ENTRY_IS_EMPTY(entry)) {
			assert(j < set->size);
			items[j++] = entry->item;

			while (entry->overflow && !ENTRY_IS_EMPTY(entry->overflow)) {
				assert(j < set->size);
				items[j++] = entry->overflow->item;
				entry = entry->overflow;
			}
		}
	}

	return items;
}

int hashset_for_each(struct hashset *set,
	int (*cbf)(void *arg, struct hashset_item *item), void *arg)
{
	struct hashset_entry *entry;
	size_t i;
	int ret;

	for (i = 0; i < set->capacity; i++) {
		entry = &set->items[i];

		if (!ENTRY_IS_EMPTY(entry)) {
			ret = cbf(arg, &entry->item);
			if (ret) {
				return 1;
			}

			while (entry->overflow && !ENTRY_IS_EMPTY(entry->overflow)) {
				ret = cbf(arg, &entry->item);
				if (ret) {
					return 1;
				}

				entry = entry->overflow;
			}
		}
	}

	return 0;
}

