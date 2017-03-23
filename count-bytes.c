#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static unsigned long freq[256];
static unsigned long bytes;
static int ret;

#define ARRAY_SIZE(x)			(sizeof(x) / sizeof((x)[0]))

static void count(FILE *fh)
{
	unsigned char buf[4096];
	size_t i, len;

	for (;;) {
		/* Read into buffer */
		len = fread(buf, 1, sizeof(buf), fh);
		if (ferror(fh)) {
			ret = 1;
			return;
		}

		/* Count bytes */
		for (i = 0; i < len; i++)
			freq[buf[i]]++;
		bytes += len;

		/* Quit on EOF */
		if (feof(fh))
			return;
	}
}

static void print_results(void)
{
	size_t i;

	printf("Total bytes: %lu\n", bytes);
	for (i = 0; i < ARRAY_SIZE(freq); i++) {
		const unsigned char ch = i;

		if (!freq[i])
			continue;

		if (isgraph(ch) || ch == ' ')
			printf("0x%02X (%c) - %lu\n",
				ch, ch, freq[i]);
		else
			printf("0x%02X     - %lu\n",
				ch, freq[i]);
	}
}

static FILE **open_files(int argc, const char *argv[])
{
	FILE **files;
	int i;

	files = malloc(argc * sizeof(FILE *));
	if (!files) {
		fprintf(stderr, "%s: unable to allocate: %s\n",
			argv[0], strerror(errno));
		return NULL;
	}

	for (i = 1; i < argc; i++) {
		FILE *fh;

		if (!strcmp(argv[i], "-")) {
			fh = stdin;
		} else {
			fh = fopen(argv[i], "rb");
			if (!fh) {
				fprintf(stderr, "%s: %s: unable to open: %s\n",
					argv[0], argv[i], strerror(errno));
				return NULL;
			}
		}
		files[i - 1] = fh;
	}
	return files;
}

int main(int argc, const char *argv[])
{
	FILE **files;
	int i, len;

	if (argc == 1) {
		len = 1;
		files = &stdin;
	} else {
		len = argc - 1;
		files = open_files(argc, argv);
		if (!files)
			return 1;
	}

	/* Read and close files */
	for (i = 0; i < len; i++) {
		count(files[i]);
		fclose(files[i]);
	}

	/* Print results */
	print_results();
	return ret;
}
