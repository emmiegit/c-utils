/*
 * str_hashmap.h
 *
 * libaishashmap
 * Copyright (c) 2016 Ammon Smith
 *
 * This file is available to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
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
void str_hashmap_for_each(str_hashmap map, void (*f)(const char *, long));
str_hashmap str_hashmap_resize(str_hashmap map, size_t new_capacity);

#define str_hashmap_is_empty(map)       ((map)->size == 0)

#endif /* __STR_HASHMAP_H */

