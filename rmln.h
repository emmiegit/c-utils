/*
 * rmln.h
 * rmln - Remove a file if it's a symbolic link or a hard link.
 *
 * Licensed under the Apache 2.0 License.
 */

#ifndef __RMLN_H
#define __RMLN_H

#include <stdbool.h>

#define DELETE_ALL_LINKS        0x3
#define DELETE_SYMBOLIC_LINKS   0x1
#define DELETE_HARD_LINKS       0x2

#define IF_DELETE_SYMLINKS(x)   ((x) & DELETE_SYMBOLIC_LINKS)
#define IF_DELETE_HARD_LINKS(x) ((x) & DELETE_HARD_LINKS)

int remove_link(const char *path, char mode, bool verbose);

#endif /* __RMLN_H */

