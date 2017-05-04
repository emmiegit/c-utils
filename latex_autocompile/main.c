/*
 * main.c
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

#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

#include "notify.h"
#include "pdf.h"
#include "util.h"

int main(int argc, const char *argv[])
{
	const char *directory;

	/* Go to directory */
	if (argc > 1)
		directory = argv[1];
	else
		directory = DEFAULT_DIRECTORY;

	if (chdir(directory))
		die("Unable to change directory to \"%s\"", directory);
	printf("Listening on \"%s\"\n", directory);

	/* Initialize */
	notify_init(directory);
	pdf_init();

	atexit(notify_cleanup);
	atexit(pdf_cleanup);

	/* Main loop */
	for (;;) {
		if (notify_read())
			fputs("Inotify event processing failed\n", stderr);
	}
}
