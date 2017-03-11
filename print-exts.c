#define _XOPEN_SOURCE	700

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
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

static const char *ignore;
static int recursive;

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

static void add_entry(const char *ext)
{
	struct entry *ent;
	size_t i;

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

static void check_extension(const char *name, int dirfd)
{
	struct stat stbuf;
	char *ptr;

	/* Only consider regular files */
	if (fstatat(dirfd, name, &stbuf, 0)) {
		fprintf(stderr,
			"Unable to stat '%s': %s\n",
			name, strerror(errno));
		exit(1);
	}
	if (!S_ISREG(stbuf.st_mode))
		return;

	ptr = strrchr(name, '.');
	if (!ptr)
		exts.none++;
	else
		add_entry(ptr + 1);
}

/* Filesystem scanning */
static void scan_dir(const char *path)
{
	struct dirent *dirent;
	DIR *dh;
	int fd;

	dh = opendir(path);
	if (!dh) {
		fprintf(stderr,
			"Unable to open directory '%s': %s\n",
			path, strerror(errno));
		exit(1);
	}
	fd = dirfd(dh);
	if (fd < 0) {
		perror("Unable to get directory descriptor");
		exit(1);
	}

	while ((dirent = readdir(dh)) != NULL)
		check_extension(dirent->d_name, fd);

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
	format = "%6u : %s\n";
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

	ignore = NULL;
	recursive = 1;
	reverse = 0;
	while ((ch = getopt(argc, argv, ":ni:r")) != -1) {
		switch (ch) {
		case 'n':
			recursive = 0;
			break;
		case 'i':
			ignore = optarg;
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
	return 0;
}
