/*
 * str_hashmap.h
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

#ifndef __STR_HASHMAP_H
#define __STR_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

struct str_hashentry_st {
    const char *key;
    struct str_hashentry_st *overflow;
    long value;
    bool is_empty;
};

struct str_hashmap_st {
    size_t size;
    size_t capacity;
    struct str_hashentry_st *items;
};

typedef struct str_hashmap_st *str_hashmap;

size_t djb2_hash(const char *str);

str_hashmap str_hashmap_init();
str_hashmap str_hashmap_init_c(size_t capacity);
void str_hashmap_destroy(str_hashmap map);
void str_hashmap_clear(str_hashmap map);
bool str_hashmap_put(str_hashmap map, const char *key, const long value);
bool str_hashmap_remove(str_hashmap map, const char *item);
bool str_hashmap_contains_key(str_hashmap map, const char *key);
bool str_hashmap_get(str_hashmap map, const char *key, long *value);
const char **str_hashmap_list_keys(str_hashmap map);
const long *str_hashmap_list_values(str_hashmap map);
str_hashmap str_hashmap_resize(str_hashmap map, size_t new_capacity);

#endif /* __STR_HASHMAP_H */

