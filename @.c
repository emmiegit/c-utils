#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(__GNUC__)
# define NORETURN	__attribute__((noreturn))
#else
# define NORETURN
#endif /* __GNUC__ */

static const char *argv0;

static struct {
	struct path {
		const char *ptr;
		size_t len;
	} *array;
	size_t len;
	size_t longest;
} paths;

static void path_setup(void)
{
	struct path *ent;
	const char *path_env, *ptr;
	size_t i;

	path_env = getenv("PATH");
	if (!path_env || !path_env[0]) {
		fprintf(stderr, "%s: no PATH variable found\n",
			argv0);
		exit(EXIT_FAILURE);
	}

	paths.len = 1;
	paths.longest = 0;
	for (i = 0; path_env[i]; i++) {
		if (path_env[i] == ':') {
			paths.len++;
		}
	}

	paths.array = malloc(sizeof(struct path) * paths.len);
	if (!paths.array) {
		fprintf(stderr, "%s: unable to allocate: %s\n",
			argv0, strerror(errno));
		exit(EXIT_FAILURE);
	}

	ent = &paths.array[0];
	ptr = path_env;
	for (i = 0; path_env[i]; i++) {
		if (path_env[i] == ':') {
			ent->ptr = ptr;
			ent->len = path_env + i - ptr;
			ptr = path_env + i + 1;

			if (paths.longest < ent->len) {
				paths.longest = ent->len;
			}
			ent++;
		}
	}
	ent->ptr = ptr;
	ent->len = path_env + i - ptr;
	if (paths.longest < ent->len) {
		paths.longest = ent->len;
	}
}

static int ok_to_execut(const char *path)
{
	struct stat stbuf;

	if (access(path, X_OK) ||
	    lstat(path, &stbuf)) {
		return 0;
	}
	if (!S_ISREG(stbuf.st_mode)) {
		return 0;
	}
	return 1;
}

static void get_execut(char **ptr)
{
	const struct path *ent;
	const char *prog;
	char *buf;
	size_t i;

	prog = *ptr;
	buf = malloc(paths.longest + strlen(prog) + 2);
	if (!buf) {
		fprintf(stderr, "%s: unable to allocate: %s\n",
			argv0, strerror(errno));
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < paths.len; i++) {
		ent = &paths.array[i];
		memcpy(buf, ent->ptr, ent->len);
		buf[ent->len] = '/';
		strcpy(buf + ent->len + 1, prog);
		if (ok_to_execut(buf)) {
			*ptr = buf;
			return;
		}
	}
	fprintf(stderr, "%s: no candidates for execution\n",
		argv0);
	exit(EXIT_FAILURE);
}

static NORETURN void do_exec(char *argv[])
{
	execv(argv[0], argv);
	fprintf(stderr, "%s: %s: unable to exec: %s\n",
		argv0, argv[0], strerror(errno));
	exit(-1);
}

/*
 * The same as executing a normal command with
 * the given arguments, except that any entries
 * from "which" are ignored if they are symbolic
 * links.
 */
int main(int argc, char *argv[])
{
	argv0 = argv[0];
	if (argc == 1) {
		fprintf(stderr, "%s: missing program name\n",
			argv0);
		return EXIT_FAILURE;
	}
	if (strchr(argv[1], '/')) {
		do_exec(argv + 1);
	}

	path_setup();
	get_execut(&argv[1]);
	do_exec(argv + 1);
}
