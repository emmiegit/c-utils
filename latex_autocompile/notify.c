/*
 * notify.c
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

#include <sys/inotify.h>
#include <limits.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "notify.h"
#include "pdf.h"
#include "util.h"

static int ifd, wd;

/* Utilities */

static void read_event(const struct inotify_event *event)
{
	const time_t now = time(NULL);

	if (event->mask == IN_IGNORED)
		return;
	if (!endswith(event->name, SOURCE_EXTENSION, SOURCE_EXTENSION_LEN))
		return;

	printf(">> %s", ctime(&now));
	if (event->mask == IN_CREATE) {
		if (compile_command(event->name))
			return;
	} else {
		puts("Directory was deleted or moved.");
		exit(0);
	}

	pdf_trigger(event->name);
}

/* Externals */

void notify_init(const char *directory)
{
	ifd = inotify_init1(IN_CLOEXEC);
	if (ifd < 0)
		die("Unable to initialize inotify");

	wd = inotify_add_watch(ifd,
			       directory,
			       IN_CREATE | IN_DELETE_SELF | IN_MOVE_SELF);
	if (wd < 0)
		die("Unable to add watch for \"%s\"", directory);
}

void notify_cleanup(void)
{
	if (inotify_rm_watch(ifd, wd))
		die("Unable to remove watch");
	if (close(ifd))
		die("Unable to stop inotify");
}

int notify_read(void)
{
	char buf[sizeof(struct inotify_event) + PATH_MAX + 1];
	ssize_t len;

	len = read(ifd, buf, sizeof(buf));
	if (len < 0) {
		if (errno == EINTR)
			return 0;
		perror("Unable to read from inotify");
		return -1;
	}
	read_event((struct inotify_event *)buf);
	return 0;
}
