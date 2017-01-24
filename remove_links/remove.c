/*
 * remove.c
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

#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "options.h"
#include "remove.h"

static int prompt(const char *const path, const mode_t mode, const unsigned int links)
{
	char response[32];

	if (S_ISDIR(mode)) {
		printf("descend into '%s'? ", path);
	} else if (links < 2) {
		printf("remove symbolic link '%s'? ", path);
	} else {
		const char *strftype;

		/* Pretty print the file type */
		if (S_ISREG(mode)) {
			strftype = "regular file";
		} else if (S_ISDIR(mode)) {
			strftype = "directory";
		} else if (S_ISLNK(mode)) {
			strftype = "symbolic link";
		} else if (S_ISFIFO(mode)) {
			strftype = "named pipe (fifo)";
		} else if (S_ISSOCK(mode)) {
			strftype = "socket";
		} else if (S_ISCHR(mode)) {
			strftype = "character device";
		} else if (S_ISBLK(mode)) {
			strftype = "block device";
		} else {
			fprintf(stderr, "%s: internal error, invalid mode: %u\n",
				opt.argv0, mode);
			abort();
		}

		printf("remove %s '%s' with %u hard links? ", strftype, path, links);
	}

	fgets(response, sizeof(response), stdin);
	return tolower(response[0]) == 'y';
}

static void print_is_a_directory(const char *const path)
{
	fprintf(stderr, "%s: won't remove '%s': is a directory\n",
		opt.argv0, path);
}

static void print_not_a_link(const char *const path)
{
	switch (opt.mode) {
	case DELETE_ALL_LINKS:
		fprintf(stderr, "%s: won't remove '%s': not a link\n", opt.argv0, path);
		break;
	case DELETE_SYMBOLIC_LINKS:
		fprintf(stderr, "%s: won't remove '%s': not a symbolic link\n", opt.argv0, path);
		break;
	case DELETE_HARD_LINKS:
		fprintf(stderr, "%s: won't remove '%s': not a hard link\n", opt.argv0, path);
	}
}

static int unlink_path(const char *const path, const mode_t mode, const unsigned int links)
{
	if (opt.interactive) {
		if (!prompt(path, mode, links)) {
			return 1;
		}
	}
	if (unlink(path) && !opt.force) {
		fprintf(stderr, "%s: cannot remove '%s': %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}
	if (opt.verbose) {
		printf("removed '%s'.\n", path);
	}
	return 0;
}

static int unlink_directory(const char *const path)
{
	DIR *dir;
	struct dirent *entry;
	int ret, status = 0;

	dir = opendir(path);
	if (!dir) {
		fprintf(stderr, "%s: cannot open '%s': %s\n",
			opt.argv0, path, strerror(errno));

		return 1;
	}

	if (opt.interactive) {
		if (!prompt(path, S_IFDIR, 0)) {
			return 0;
		}
	}

	while ((entry = readdir(dir))) {
		char *fullpath;

		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
			continue;
		}

		fullpath = malloc(strlen(path) + strlen(entry->d_name) + 2);
		if (!fullpath) {
			fprintf(stderr, "%s: cannot allocate pathname buffer: %s\n",
				opt.argv0, strerror(errno));
			return 1;
		}

		sprintf(fullpath, "%s/%s", path, entry->d_name);

		if (entry->d_type == DT_DIR) {
			status += unlink_directory(fullpath);
		} else if (entry->d_type == DT_LNK && IF_DELETE_SYMLINKS(opt.mode)) {
			status += unlink_path(fullpath, S_IFLNK, 0);
		} else {
			status += remove_recursive(fullpath);
		}

		free(fullpath);
	}

	ret = closedir(dir);
	if (ret) {
		fprintf(stderr, "%s: cannot close '%s': %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}

	return status;
}

int remove_link(const char *const path)
{
	struct stat statbuf;
	if (lstat(path, &statbuf)) {
		if (!opt.argv0) puts("prog null");
		if (!path) puts("path null");
		fprintf(stderr, "%s: unable to stat '%s': %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}

	if (S_ISDIR(statbuf.st_mode)) {
		print_is_a_directory(path);
		return 1;
	} else if (statbuf.st_nlink > 1 && IF_DELETE_HARD_LINKS(opt.mode)) {
		return unlink_path(path, statbuf.st_mode, statbuf.st_nlink);
	} else if (S_ISLNK(statbuf.st_mode) && IF_DELETE_SYMLINKS(opt.mode)) {
		return unlink_path(path, statbuf.st_mode, 0);
	}

	print_not_a_link(path);
	return 1;
}

int remove_recursive(const char *const path)
{
	struct stat statbuf;
	if (lstat(path, &statbuf)) {
		fprintf(stderr, "%s: unable to stat '%s': %s\n",
			opt.argv0, path, strerror(errno));
		return 1;
	}

	if (S_ISDIR(statbuf.st_mode)) {
		return unlink_directory(path);
	} else if (statbuf.st_nlink > 1 && IF_DELETE_HARD_LINKS(opt.mode)) {
		return unlink_path(path, statbuf.st_mode, statbuf.st_nlink);
	} else if (S_ISLNK(statbuf.st_mode) && IF_DELETE_SYMLINKS(opt.mode)) {
		return unlink_path(path, statbuf.st_mode, 0);
	}

	print_not_a_link(path);
	return 1;
}
