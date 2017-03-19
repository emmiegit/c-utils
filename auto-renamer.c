#define _XOPEN_SOURCE 500

#include <sys/inotify.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_DIRECTORY	"."
#define INOTIFY_BUFFER_SIZE	(sizeof(struct inotify_event) + PATH_MAX + 1)

#define UNUSED(x)		((void)(x))
#define MAX(x,y)		(((x) > (y)) ? (x) : (y))

struct max_filename {
	unsigned int number;
	unsigned int width;
};

static unsigned int int_digits(unsigned int x)
{
	if (x > 1000000000u)
		return 9;
	else if (x > 100000000u)
		return 8;
	else if (x > 10000000u)
		return 7;
	else if (x > 1000000u)
		return 6;
	else if (x > 100000u)
		return 5;
	else if (x > 10000u)
		return 4;
	else if (x > 1000u)
		return 3;
	else if (x > 100u)
		return 2;
	else if (x > 10u)
		return 1;
	else
		return 0;
}

static size_t filename_len(const char *str, size_t len)
{
	const char *end;

	end = str + len - 1;
	while (end >= str) {
		if (*end == '.')
			break;
		end--;
	}
	if (str > end)
		return len;
	else
		return end - str;
}

static void update_max(struct max_filename *maxf, const char *fn)
{
	size_t i, fnlen;
	unsigned int total;

	/* Find last period, if any */
	fnlen = filename_len(fn, strlen(fn));

	/* If it's an integer, read its value */
	total = 0;
	for (i = 0; i < fnlen; i++) {
		if (!isdigit(fn[i]))
			return;

		total *= 10;
		total += fn[i] - '0';
	}

	maxf->number = MAX(maxf->number, total);
	maxf->width = MAX(maxf->width, fnlen);
}

static void rename_file(const char *fn,
			size_t flen,
			const struct max_filename *maxf)
{
	char fmt[64];
	char *buf;
	const char *ext;
	size_t len, fnlen;

	fnlen = filename_len(fn, flen);
	ext = fn + fnlen;
	len = (flen - fnlen) + maxf->width;

	buf = malloc(len + 1);
	if (!buf) {
		perror("Unable to allocate memory");
		return;
	}

	snprintf(fmt, sizeof(fmt), "%%0%uu%%s", maxf->width);
	sprintf(buf, fmt, maxf->number, ext);

	/*
	 * Don't rename if the original and destination
	 * are the same.
	 */
	if (!strcmp(fn, buf))
		return;

	/* Rename files */
	printf("Renaming \"%s\" -> \"%s\"\n", fn, buf);
	if (rename(fn, buf)) {
		fprintf(stderr, "Unable to rename \"%s\" -> \"%s\"\n",
			fn, buf);
		free(buf);
		return;
	}
	free(buf);
}

static void read_event(const struct inotify_event *evt)
{
	struct max_filename maxf;
	const struct dirent *dirent;
	unsigned int digits;
	DIR *dh;

	/* Exit if the directory isn't in place any more */
	if (evt->mask != IN_CREATE) {
		puts("Directory was deleted or moved.");
		exit(EXIT_SUCCESS);
	}

	maxf.number = 0;
	maxf.width = 2;

	dh = opendir(".");
	if (!dh) {
		perror("Unable to open current directory");
		return;
	}

	/* Get highest numbered file */
	while ((dirent = readdir(dh)) != NULL) {
		if (!strcmp(dirent->d_name, evt->name))
			continue;
		update_max(&maxf, dirent->d_name);
	}

	digits = int_digits(maxf.number);
	maxf.number += 1;
	maxf.width = MAX(maxf.width, digits);

	/* Rename new file */
	rename_file(evt->name, evt->len, &maxf);
}

int main(int argc, const char *argv[])
{
	const char *directory;
	int inotify_fd, wd;

	directory = DEFAULT_DIRECTORY;
	if (argc > 1)
		directory = argv[1];

	if (chdir(directory)) {
		fprintf(stderr, "Unable to change directory to \"%s\": %s\n",
			directory, strerror(errno));
		return EXIT_FAILURE;
	}

	inotify_fd = inotify_init();
	if (inotify_fd < 0) {
		perror("Unable to initialize inotify");
		return EXIT_FAILURE;
	}

	wd = inotify_add_watch(inotify_fd, directory, IN_CREATE | IN_DELETE_SELF | IN_MOVE_SELF);
	if (wd < 0) {
		fprintf(stderr, "Unable to add watch for \"%s\": %s\n",
			directory, strerror(errno));
		return EXIT_FAILURE;
	}

	printf("Listening on \"%s\"\n", directory);
	for (;;) {
		union {
			char buf[INOTIFY_BUFFER_SIZE];
			struct inotify_event evt;
		} u;
		ssize_t len;

		len = read(inotify_fd, u.buf, sizeof(u.buf));
		if (len < 0) {
			if (errno == EINTR)
				break;
			perror("Unable to read from inotify");
			return EXIT_FAILURE;
		}
		read_event(&u.evt);
	}
	return 0;
}
