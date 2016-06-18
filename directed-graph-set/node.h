/*
 * node.h
 *
 * libaisdgs (ais directed-graph-set)
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

#ifndef __NODE_H
#define __NODE_H

#include <stddef.h>

struct node_reference {
    struct graph_node *node;
    unsigned int depth;
};

struct graph_node {
    union node_value {
        void *pointer;
        long long_value;
        int int_value;
        short short_value;
        char char_value;
    } value;

    struct node_list {
        struct node_reference *list;
        size_t length;
        size_t capacity;
    } next_nodes;
};

struct graph_node *graph_node_init(struct graph_node *node);
struct graph_node *graph_node_init_c(struct graph_node *node, size_t capacity);
void graph_node_destroy(struct graph_node *node);

#endif /* __NODE_H */

/* vim: set ts=8 sw=4 ft=c: */
