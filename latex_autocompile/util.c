/*
 * util.c
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

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

/* String functions */

int endswith(const char *str, const char *suffix, size_t suflen)
{
	size_t i, len;

	len = strlen(str);
	for (i = 0; i < suflen; i++) {
		if (str[i + len - suflen] != suffix[i])
			return 0;
	}
	return 1;
}

char *replace_ext(const char *str, const char *ext, size_t extlen)
{
	size_t i, len;
	char *new;

	len = strlen(str);
	assert(extlen <= len);
	new = malloc(len + 1);
	if (!new)
		die("Unable to allocate memory");

	memcpy(new, str, len - extlen);
	for (i = 0; i < extlen; i++)
		new[i + len - extlen] = ext[i];
	new[len] = '\0';
	return new;
}

/* Process functions */

int compile_command(const char *filename)
{
	pid_t pid;
	int ret;

	if ((pid = FORK()) < 0)
		die("Unable to fork");

	if (pid == 0) {
		/* We are the child */
		close(STDIN_FILENO);
		execlp(COMPILE_COMMAND, COMPILE_COMMAND, filename, (char *)NULL);
		_exit(-1);
	} else {
		waitpid(pid, &ret, 0);
	}
	return ret;
}
