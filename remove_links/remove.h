/*
 * remove.h
 *
 * rmln - Only remove links.
 * Copyright (c) 2016 Ammon Smith
 *
 * rmln is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * rmln is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rmln.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __REMOVE_H
#define __REMOVE_H

#include "options.h"

int remove_link(const char *path);
int remove_recursive(const char *const path);

#endif /* __REMOVE_H */

