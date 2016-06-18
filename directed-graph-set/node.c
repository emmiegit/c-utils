/*
 * node.c
 *
 * directed-graph-set
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

#include <stdlib.h>

#include "node.h"

#define DEFAULT_NODE_LIST_CAPACITY      10

struct graph_node *graph_node_init(struct graph_node *node)
{
    return graph_node_init_c(node, DEFAULT_NODE_LIST_CAPACITY);
}

struct graph_node *graph_node_init_c(struct graph_node *node, size_t capacity)
{
    node->next_nodes.list = malloc(sizeof(struct node_reference) * capacity);
    node->next_nodes.length = 0;
    node->next_nodes.capacity = capacity;
    if (node->next_nodes.list == NULL) {
        return NULL;
    }

    return node;
}

void graph_node_destroy(struct graph_node *node)
{
    free(node->next_nodes.list);
}

/* vim: set ts=8 sw=4 ft=c: */
