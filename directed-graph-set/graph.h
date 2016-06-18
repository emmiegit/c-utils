/*
 * graph.h
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

#ifndef __GRAPH_H
#define __GRAPH_H

#include "node.h"

struct graph {
    struct graph_node *nodes;
    size_t length;
    size_t capacity;
};

struct graph *graph_init();
struct graph *graph_init_c(size_t capacity);
void graph_destroy(struct graph *graph);

#endif /* __GRAPH_H */

/* vim: set ts=8 sw=4 ft=c: */
