/*
 * int_hashmap.h
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

#ifndef __INT_HASHMAP_H
#define __INT_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

struct int_hashentry_st {
    long key;
    const void *value;
    struct int_hashentry_st *overflow;
    bool is_empty;
};

struct int_hashmap_st {
    size_t size;
    size_t capacity;
    struct int_hashentry_st *items;
};

typedef struct int_hashmap_st *int_hashmap;

int_hashmap int_hashmap_init();
int_hashmap int_hashmap_init_c(size_t capacity);
void int_hashmap_destroy(int_hashmap map);
void int_hashmap_clear(int_hashmap map);
bool int_hashmap_put(int_hashmap map, long key, const void *value);
bool int_hashmap_remove(int_hashmap map, long item);
bool int_hashmap_contains_key(int_hashmap map, long key);
bool int_hashmap_get(int_hashmap map, long key, const void **value);
const long *int_hashmap_list_keys(int_hashmap map);
const void **int_hashmap_list_values(int_hashmap map);
void int_hashmap_for_each(int_hashmap map, void (*f)(long, const void *));
int_hashmap int_hashmap_resize(int_hashmap map, size_t new_capacity);

#define int_hashmap_is_empty(map)       ((map)->size == 0)

#endif /* __INT_HASHMAP_H */

