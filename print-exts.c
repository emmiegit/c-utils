#define _XOPEN_SOURCE	700

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct {
	struct entry {
		char ext[64];
		unsigned int count;
	} *array;
	unsigned int none;

	size_t len;
	size_t capacity;
} exts;

static struct {
	const char **array;
	size_t len;
} ignore;

static int debug, recursive;
static unsigned int depth;

/* Returns true if x is '.' or '..' */
#define IS_DOT(x)					\
	(((x)[0] == '.') && ((x)[1] == '.' || (x)[1] == '\0'))

#define DEBUG(action, path)				\
	do {						\
		unsigned int __i;			\
		if (!debug)				\
			break;				\
		for (__i = 0; __i < depth; __i++)	\
			putchar(' ');			\
		printf("%s '%s'\n", (action), (path));	\
	} while (0)

/* Ignored files */
static void add_ignore(const char *path)
{
	void *ptr;

	ptr = realloc(ignore.array, (ignore.len + 1) * sizeof(const char *));
	if (!ptr) {
		perror("Couldn't allocate buffer");
		exit(1);
	}
	ignore.array = ptr;
	ignore.array[ignore.len++] = path;
}

static int check_ignore(const char *path)
{
	size_t i;

	for (i = 0; i < ignore.len; i++) {
		if (!strcmp(ignore.array[i], path)) {
			DEBUG("ignoring", path);
			return 1;
		}
	}
	return 0;
}

/* Data manipulation */
static int compare(const void *_x, const void *_y)
{
	const struct entry *x, *y;

	x = _x;
	y = _y;
	if (x->count != y->count)
		return (int)y->count - (int)x->count;
	else
		return strcmp(x->ext, y->ext);
}

static void resize(void)
{
	void *ptr;
	size_t new_capacity;

	if (exts.capacity == 0)
		new_capacity = 8;
	else
		new_capacity = exts.capacity * 2;
	ptr = realloc(exts.array, new_capacity * sizeof(struct entry));
	if (!ptr) {
		perror("Couldn't allocate buffer");
		exit(1);
	}
	exts.array = ptr;
	exts.capacity = new_capacity;
}

static void count_file(const char *name)
{
	struct entry *ent;
	const char *ext;
	char *ptr;
	size_t i;

	ptr = strrchr(name, '.');
	if (!ptr) {
		exts.none++;
		return;
	}

	ext = ptr + 1;
	for (i = 0; i < exts.len; i++) {
		ent = &exts.array[i];
		if (ext == ent->ext || !strcmp(ext, ent->ext)) {
			/* Increment existing */
			ent->count++;
			return;
		}
	}
	if (exts.len >= exts.capacity)
		resize();

	/* Add new extension */
	ent = &exts.array[exts.len++];
	strncpy(ent->ext, ext, sizeof(ent->ext));
	ent->count = 1;
}

/* Filesystem scanning */
static void scan_dir(const char *path, int fd)
{
	struct dirent *dirent;
	DIR *dh;

	DEBUG("entering", path);
	fd = openat(fd, path, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr,
			"Unable to open handle for '%s': %s\n",
			path, strerror(errno));
		exit(1);
	}
	dh = fdopendir(fd);
	if (!dh) {
		fprintf(stderr,
			"Unable to open directory '%s': %s\n",
			path, strerror(errno));
		exit(1);
	}
	depth++;

	while ((dirent = readdir(dh)) != NULL) {
		struct stat stbuf;

		if (check_ignore(dirent->d_name) || IS_DOT(dirent->d_name))
			continue;
		DEBUG("stat", dirent->d_name);
		if (fstatat(fd, dirent->d_name, &stbuf, 0)) {
			fprintf(stderr,
				"Unable to stat '%s': %s\n",
				dirent->d_name, strerror(errno));
			continue;
		}
		if (S_ISREG(stbuf.st_mode)) {
			DEBUG("counting", dirent->d_name);
			count_file(dirent->d_name);
		} else if (S_ISDIR(stbuf.st_mode) && recursive) {
			scan_dir(dirent->d_name, fd);
		}
	}

	depth--;
	DEBUG("leaving", path);
	if (closedir(dh)) {
		perror("Unable to close directory");
		exit(1);
	}
}

/* Output */
static void print_result(int reverse)
{
	const char *format;
	size_t i;
	int none_flag;

	if (!exts.len && !exts.none) {
		puts("No files found.");
		exit(0);
	}

	qsort(exts.array, exts.len, sizeof(struct entry), compare);
	format = "%8u %s\n";
	none_flag = 1;

	if (reverse) {
		for (i = 0; i < exts.len; i++) {
			const struct entry *ent;

			ent = &exts.array[exts.len - i - 1];
			if (exts.none < ent->count && none_flag) {
				printf(format, exts.none, "(none)");
				none_flag = 0;
			}
			printf(format, ent->count, ent->ext);
		}
	} else {
		for (i = 0; i < exts.len; i++) {
			const struct entry *ent;

			ent = &exts.array[i];
			if (exts.none > ent->count && none_flag) {
				printf(format, exts.none, "(none)");
				none_flag = 0;
			}
			printf(format, ent->count, ent->ext);
		}
	}
}

int main(int argc, char *argv[])
{
	int reverse;
	int ch, i;

	recursive = 1;
	reverse = 0;
	while ((ch = getopt(argc, argv, "dhNi:r")) != -1) {
		switch (ch) {
		case 'd':
			debug = 1;
			break;
		case 'h':
			printf("Usage: %s [-d] [-N] [-i FILE] [-r] DIR...\n"
				"Usage: %s -h\n"
				" -h        Print this help message\n"
				" -d        Enable debug mode\n"
				" -N        Disable recursion\n"
				" -i FILE   Ignore the given file.\n"
				"           (Can be given multiple times)\n"
				" -r        Print in reverse order.\n",
				argv[0], argv[0]);
			exit(0);
		case 'N':
			recursive = 0;
			break;
		case 'i':
			add_ignore(optarg);
			break;
		case 'r':
			reverse = 1;
			break;
		case '?':
			return 1;
		default:
			abort();
		}
	}

	if (optind == argc)
		scan_dir(".", AT_FDCWD);
	else for (i = optind; i < argc; i++)
		scan_dir(argv[i], AT_FDCWD);
	print_result(reverse);
	return 0;
}
