#define _XOPEN_SOURCE	700

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <ftw.h>
#include <libgen.h>
#include <limits.h>
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

static int recursive;
static int ret;

#define UNUSED(x)	((void)(x))
#define MAX_FD		32

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
static int scan_cbf(const char *fpath,
		    const struct stat *stbuf,
		    int ftype)
{
	char _name[PATH_MAX], *name;

	UNUSED(stbuf);

	strcpy(_name, fpath);
	name = basename(_name);

	if (check_ignore(name))
		return 0;

	switch (ftype) {
	case FTW_F:
		count_file(name);
		break;
	case FTW_D:
		/* nothing to do */
		break;
	case FTW_DNR:
	case FTW_NS:
	case FTW_SLN:
		fprintf(stderr,
			"Unable to open file: %s\n",
			fpath);
		break;
	default:
		abort();
	}
	return 0;
}

static void scan_dir(const char *path)
{
	if (ftw(path, scan_cbf, MAX_FD)) {
		fputs("Errors while iterating.\n", stderr);
		ret = 1;
		return;
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
			printf(format, ent->count, ent->ext);
			if (exts.none < ent->count && none_flag) {
				printf(format, exts.none, "(none)");
				none_flag = 0;
			}
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
	if (none_flag)
		printf(format, exts.none, "(none)");
}

int main(int argc, char *argv[])
{
	int reverse;
	int ch, i;

	recursive = 1;
	reverse = 0;
	while ((ch = getopt(argc, argv, "dhNi:r")) != -1) {
		switch (ch) {
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
		scan_dir(".");
	else for (i = optind; i < argc; i++)
		scan_dir(argv[i]);
	print_result(reverse);
	return ret;
}
