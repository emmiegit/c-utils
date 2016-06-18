/*
 * graph.c
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

#include "graph.h"

#define DEFAULT_GRAPH_NODE_LIST_CAPACITY  20

struct graph *graph_init()
{
    return graph_init_c(DEFAULT_GRAPH_NODE_LIST_CAPACITY);
}

struct graph *graph_init_c(size_t capacity)
{
    struct graph *graph = malloc(sizeof(struct graph));
    if (graph == NULL) {
        return NULL;
    }

    graph->nodes = malloc(sizeof(struct graph_node) * capacity);
    graph->length = 0;
    graph->capacity = capacity;
    if (graph->nodes == NULL) {
        free(graph);
        return NULL;
    }

    return graph;
}

void graph_destroy(struct graph *graph)
{
    size_t i;
    for (i = 0; i < graph->length; i++) {
        graph_node_destroy(&graph->nodes[i]);
    }

    free(graph->nodes);
    free(graph);
}

/* vim: set ts=8 sw=4 ft=c: */
