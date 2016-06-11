/*
 * str_hashmap.c
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
#include <string.h>

#include "str_hashmap.h"

#define STREQUALS(x, y)      ((x) && (y) && (strcmp((x), (y)) == 0))

#define DEFAULT_MAP_CAPACITY 20

static void str_hashentry_destroy(struct str_hashentry_st *entry);

size_t djb2_hash(const char *str)
{
    if (str == NULL) {
        return 0;
    }

    size_t i,  hash = 5381;
    for (i = 0; str[i]; i++) {
        /* hash * 33 + ch */
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;
}

str_hashmap str_hashmap_init()
{
    return str_hashmap_init_c(DEFAULT_MAP_CAPACITY);
}

str_hashmap str_hashmap_init_c(size_t capacity)
{
    if (capacity == 0) {
        return NULL;
    }

    str_hashmap map = malloc(sizeof(struct str_hashmap_st));

    if (map == NULL) {
        return NULL;
    }

    map->size = 0;
    map->capacity = DEFAULT_MAP_CAPACITY;
    map->items = malloc(map->capacity * sizeof(struct str_hashentry_st));

    if (map->items == NULL) {
        str_hashmap_destroy(map);
        return NULL;
    }

    size_t i;
    for (i = 0; i < map->capacity; i++) {
        map->items[i].overflow = NULL;
        map->items[i].is_empty = true;
    }

    return map;
}

void str_hashmap_destroy(str_hashmap map)
{
    if (map) {
        if (map->items) {
            size_t i;
            for (i = 0; i < map->capacity; i++) {
                str_hashentry_destroy(map->items[i].overflow);
            }

            free(map->items);
        }

        free(map);
    }
}

static void str_hashentry_destroy(struct str_hashentry_st *entry)
{
    if (entry) {
        if (entry->overflow) {
            str_hashentry_destroy(entry->overflow);
        }

        free(entry);
    }
}

void str_hashmap_clear(str_hashmap map)
{
    map->size = 0;
    size_t i;
    for (i = 0; i < map->capacity; i++) {
        map->items[i].is_empty = true;
        str_hashentry_destroy(map->items[i].overflow);
    }
}

bool str_hashmap_put(str_hashmap map, const char *key, long value)
{
    size_t index = djb2_hash(key) % map->capacity;
    struct str_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        map->size++;
        entry->key = key;
        entry->value = value;
        entry->is_empty = false;
        return true;
    } else if (key == entry->key || STREQUALS(key, entry->key)) {
        entry->value = value;
        return true;
    }

    do {
        if (entry->overflow == NULL) {
            entry->overflow = malloc(sizeof(struct str_hashentry_st));

            if (entry->overflow == NULL) {
                str_hashmap_destroy(map);
                return false;
            }

            map->size++;
            entry->overflow->key = key;
            entry->overflow->value = value;
            entry->overflow->overflow = NULL;
            entry->overflow->is_empty = false;
            return true;
        } else if (entry->overflow->is_empty) {
            map->size++;
            entry->overflow->key = key;
            entry->overflow->value = value;
            entry->overflow->is_empty = false;
        } else if (key == entry->overflow->key || STREQUALS(key, entry->overflow->key)) {
            entry->overflow->value = value;
            return true;
        }

        entry = entry->overflow;
    } while (entry->overflow);

    return false;
}

bool str_hashmap_remove(str_hashmap map, const char *key)
{
    size_t index = djb2_hash(key) % map->capacity;
    struct str_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        return false;
    }

    if (key == entry->key || STREQUALS(key, entry->key)) {
        if (entry->overflow) {
            entry->key = entry->overflow->key;
            entry->value = entry->overflow->value;
            entry->is_empty = entry->overflow->is_empty;
            entry->overflow = entry->overflow->overflow;
        } else {
            entry->is_empty = true;
        }

        map->size--;
        return true;
    } else if (entry->overflow == NULL) {
        return false;
    } else {
        while (entry->overflow && entry->overflow->is_empty) {
            entry = entry->overflow;

            if (key == entry->key || STREQUALS(key, entry->key)) {
                if (entry->overflow) {
                    entry->key = entry->overflow->key;
                    entry->value = entry->overflow->value;
                    entry->is_empty = entry->overflow->is_empty;
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

bool str_hashmap_contains_key(str_hashmap map, const char *key)
{
    size_t index = djb2_hash(key) % map->capacity;
    struct str_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        return false;
    } else if (key == entry->key || STREQUALS(key, entry->key)) {
        return true;
    } else if (entry->overflow == NULL || entry->overflow->is_empty) {
        return false;
    } else {
        while (entry->overflow && !entry->overflow->is_empty) {
            entry = entry->overflow;

            if (key == entry->key || STREQUALS(key, entry->key)) {
                return true;
            }
        }

        return false;
    }
}

bool str_hashmap_get(str_hashmap map, const char *key, long *value)
{
    size_t index = djb2_hash(key) % map->capacity;
    struct str_hashentry_st *entry = &map->items[index];

    if (entry->is_empty) {
        return false;
    } else if (key == entry->key || STREQUALS(key, entry->key)) {
        *value = entry->value;
        return true;
    } else if (entry->overflow == NULL) {
        return false;
    } else {
        while (entry->overflow) {
            entry = entry->overflow;

            if (key == entry->key || STREQUALS(key, entry->key)) {
                *value = entry->value;
                return true;
            }
        }

        return false;
    }
}

const char **str_hashmap_list_keys(str_hashmap map)
{
    const char **array = malloc(map->size * sizeof(char *));

    struct str_hashentry_st *entry;
    size_t i, j = 0;
    for (i = 0; i < map->capacity; i++) {
        entry = &map->items[i];
        if (!entry->is_empty) {
            assert(j < map->size);
            array[j++] = entry->key;

            while (entry->overflow && !entry->overflow->is_empty) {
                assert(j < map->size);
                entry = entry->overflow;
                array[j++] = entry->key;
            }
        }
    }

    return array;
}

const long *str_hashmap_list_values(str_hashmap map)
{
    long *array = malloc(map->size * sizeof(char *));

    struct str_hashentry_st *entry;
    size_t i, j = 0;
    for (i = 0; i < map->capacity; i++) {
        entry = &map->items[i];
        if (!entry->is_empty) {
            array[j++] = entry->value;

            while (entry->overflow && !entry->overflow->is_empty) {
                entry = entry->overflow;
                array[j++] = entry->value;
            }
        }
    }

    return array;
}

void str_hashmap_for_each(str_hashmap map, void (*f)(const char *, long))
{
    struct str_hashentry_st *entry;
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

str_hashmap str_hashmap_resize(str_hashmap map, size_t new_capacity)
{
    if (new_capacity == 0) {
        return NULL;
    }

    str_hashmap new_map = str_hashmap_init();
    if (new_map == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < map->capacity; i++) {
        struct str_hashentry_st *entry = &map->items[i];

        if (!entry->is_empty) {
            str_hashmap_put(new_map, entry->key, entry->value);

            while (entry->overflow && !entry->overflow->is_empty) {
                str_hashmap_put(new_map, entry->overflow->key, entry->overflow->value);
                entry = entry->overflow;
            }
        }

        str_hashentry_destroy(entry);
    }

    free(map->items);
    map->items = new_map->items;
    map->capacity = new_capacity;
    assert(map->size == new_map->size);
    free(new_map);

    return map;
}

