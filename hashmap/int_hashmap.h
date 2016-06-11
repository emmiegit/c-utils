/*
 * int_hashmap.c
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

