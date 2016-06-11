/*
 * int_hashmap.c
 *
 * antz - My fictional assembly language and emulator.
 * Copyright (c) 2016 Ammon Smith
 *
 * antz is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * antz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with antz.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <stdlib.h>

#include "int_hashmap.h"

#define HASH_KEY(key, cap)   ((((key) * (key)) ^ 0x176a) % (cap))

#define DEFAULT_MAP_CAPACITY 5

static void int_hashentry_destroy(struct int_hashentry_st *entry);

int_hashmap int_hashmap_init()
{
    return int_hashmap_init_c(DEFAULT_MAP_CAPACITY);
}

int_hashmap int_hashmap_init_c(size_t capacity)
{
    if (capacity == 0) {
        return NULL;
    }

    int_hashmap map = malloc(sizeof(struct int_hashmap_st));

    if (map == NULL) {
        return NULL;
    }

    map->size = 0;
    map->capacity = capacity;
    map->items = malloc(map->capacity * sizeof(struct int_hashentry_st));

    if (map->items == NULL) {
        int_hashmap_destroy(map);
        return NULL;
    }

    size_t i;
    for (i = 0; i < map->capacity; i++) {
        map->items[i].overflow = NULL;
        map->items[i].is_empty = true;
    }

    return map;
}

void int_hashmap_destroy(int_hashmap map)
{
    if (map) {
        if (map->items) {
            size_t i;
            for (i = 0; i < map->capacity; i++) {
                int_hashentry_destroy(map->items[i].overflow);
            }

            free(map->items);
        }

        free(map);
    }
}

static void int_hashentry_destroy(struct int_hashentry_st *entry)
{
    if (entry) {
        if (entry->overflow) {
            int_hashentry_destroy(entry->overflow);
        }

        free(entry);
    }
}

void int_hashmap_clear(int_hashmap map)
{
    map->size = 0;
    size_t i;
    for (i = 0; i < map->capacity; i++) {
        map->items[i].is_empty = true;
        int_hashentry_destroy(map->items[i].overflow);
    }
}

bool int_hashmap_put(int_hashmap map, long key, const void *value)
{
    size_t index = HASH_KEY(key, map->capacity);
    struct int_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        map->size++;
        entry->key = key;
        entry->value = value;
        entry->is_empty = false;
        return true;
    } else if (key == entry->key) {
        entry->value = value;
        return true;
    }

    do {
        if (entry->overflow == NULL) {
            entry->overflow = malloc(sizeof(struct int_hashentry_st));

            if (entry->overflow == NULL) {
                int_hashmap_destroy(map);
                return false;
            }

            map->size++;
            entry->overflow->key = key;
            entry->overflow->value = value;
            entry->overflow->is_empty = false;
            entry->overflow->overflow = NULL;
            return true;
        } else if (entry->overflow->is_empty) {
            map->size++;
            entry->overflow->key = key;
            entry->overflow->value = value;
            entry->overflow->is_empty = false;
            return true;
        } else if (key == entry->overflow->key) {
            entry->overflow->value = value;
            return true;
        }

        entry = entry->overflow;
    } while (entry->overflow);

    assert(false);
    return false;
}

bool int_hashmap_remove(int_hashmap map, long key)
{
    size_t index = HASH_KEY(key, map->capacity);
    struct int_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        return false;
    } else if (key == entry->key) {
        if (entry->overflow) {
            entry->key = entry->overflow->key;
            entry->value = entry->overflow->value;
            entry->overflow = entry->overflow->overflow;
        } else {
            entry->is_empty = true;
        }

        map->size--;
        return true;
    } else if (entry->overflow == NULL) {
        return false;
    } else {
        while (entry->overflow && !entry->overflow->is_empty) {
            entry = entry->overflow;

            if (key == entry->key) {
                if (entry->overflow) {
                    entry->key = entry->overflow->key;
                    entry->value = entry->overflow->value;
                    entry->overflow = entry->overflow->overflow;
                } else {
                    entry->is_empty = true;
                }

                map->size--;
                return true;
            }
        }

        return false;
    }
}

bool int_hashmap_contains_key(int_hashmap map, long key)
{
    size_t index = HASH_KEY(key, map->capacity);
    struct int_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        return false;
    } else if (key == entry->key) {
        return true;
    } else if (entry->overflow == NULL) {
        return false;
    } else {
        while (entry->overflow && !entry->overflow->is_empty) {
            entry = entry->overflow;

            if (key == entry->key) {
                return true;
            }
        }

        return false;
    }
}

bool int_hashmap_get(int_hashmap map, long key, const void **value)
{
    size_t index = HASH_KEY(key, map->capacity);
    struct int_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        return false;
    } else if (key == entry->key) {
        *value = entry->value;
        return true;
    } else if (entry->overflow == NULL) {
        return false;
    } else {
        while (entry->overflow && !entry->overflow->is_empty) {
            entry = entry->overflow;

            if (key == entry->key) {
                *value = entry->value;
                return true;
            }
        }

        return false;
    }
}

const long *int_hashmap_list_keys(int_hashmap map)
{
    long *array = malloc(map->size * sizeof(long));
    if (array == NULL) {
        return NULL;
    }

    struct int_hashentry_st *entry;
    size_t i, j;
    for (i = 0, j = 0; i < map->capacity; i++) {
        entry = &map->items[i];
        if (!entry->is_empty) {
            array[j++] = entry->key;

            while (entry->overflow && !entry->overflow->is_empty) {
                entry = entry->overflow;
                array[j++] = entry->key;
            }
        }
    }

    assert(j == map->size);
    return array;
}

const void **int_hashmap_list_values(int_hashmap map)
{
    const void **array = malloc(map->size * sizeof(void *));
    if (array == NULL) {
        return NULL;
    }

    struct int_hashentry_st *entry;
    size_t i, j;
    for (i = 0, j = 0; i < map->capacity; i++) {
        entry = &map->items[i];
        if (!entry->is_empty) {
            assert(j < map->size);
            array[j++] = entry->value;

            while (entry->overflow && !entry->overflow->is_empty) {
                assert(j < map->size);
                entry = entry->overflow;
                array[j++] = entry->value;
            }
        }
    }

    assert(j == map->size);
    return array;
}

void int_hashmap_for_each(int_hashmap map, void (*f)(long, const void *))
{
    struct int_hashentry_st *entry;
    size_t i;
    for (i = 0; i < map->capacity; i++) {
        entry = &map->items[i];
        if (!entry->is_empty) {
            f(entry->key, entry->value);

            while (entry->overflow && !entry->overflow->is_empty) {
                entry = entry->overflow;
                f(entry->key, entry->value);
            }
        }
    }
}

int_hashmap int_hashmap_resize(int_hashmap map, size_t new_capacity)
{
    if (new_capacity < 2) {
        return NULL;
    }

    int_hashmap new_map = int_hashmap_init();
    if (new_map == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < map->capacity; i++) {
        struct int_hashentry_st *entry = &map->items[i];

        if (!entry->is_empty) {
            int_hashmap_put(new_map, entry->key, entry->value);

            while (entry->overflow && !entry->overflow->is_empty) {
                entry = entry->overflow;
                int_hashmap_put(new_map, entry->key, entry->value);
            }
        }

        int_hashentry_destroy(entry);
    }

    free(map->items);
    map->items = new_map->items;
    map->capacity = new_capacity;
    assert(map->size == new_map->size);
    free(new_map);

    return map;
}

