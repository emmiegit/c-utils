/*
 * notify.h
 *
 * latex-autocompile - Automatically recompile LaTeX sources and
 *                     refresh the MuPDF viewer
 * Copyright (c) 2016-2017 Ammon Smith
 *
 * latex-autocompile is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * latex-autocompile is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with latex-autocompile.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _NOTIFY_H_
#define _NOTIFY_H_

#include <stdbool.h>

#include "core.h"

struct notify_settings {
	bool open_pdf   : 1;
	bool reload_pdf : 1;
};

void notify_init(const char *directory, const struct notify_settings *opt);
void notify_cleanup(void);
int notify_read(void);

#endif /* _NOTIFY_H_ */

