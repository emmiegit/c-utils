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
		if (feof(fh))
			return;

		/* Count bytes */
		for (i = 0; i < len; i++)
			freq[buf[i]]++;
		bytes += len;
	}
}

static void print_results(void)
{
	size_t i;

	printf("Total bytes: %lu\n", bytes);
	for (i = 0; i < ARRAY_SIZE(freq); i++) {
		unsigned char ch;

		if (!freq[i])
			continue;

		ch = i;
		if (isgraph(ch) || ch == ' ')
			printf("0x%02X (%c) - %lu\n",
				ch, ch, freq[i]);
		else
			printf("0x%02X     - %lu\n",
				ch, freq[i]);
	}
}

int main(int argc, const char *argv[])
{
	FILE **files;
	int i;

	/* Allocate (FILE *) array */
	files = malloc(argc * sizeof(FILE *));
	if (!files) {
		fprintf(stderr, "%s: unable to allocate: %s\n",
			argv[0], strerror(errno));
		return 1;
	}

	/* Open files */
	if (argc == 1) {
		files[0] = stdin;
	} else for (i = 1; i < argc; i++) {
		FILE *fh;

		fh = fopen(argv[i], "rb");
		if (!fh) {
			fprintf(stderr, "%s: %s: unable to open: %s\n",
				argv[0], argv[i], strerror(errno));
			return 1;
		}
		files[i - 1] = fh;
	}

	/* Read and close files */
	for (i = 0; i < argc - 1; i++) {
		count(files[i]);
		fclose(files[i]);
	}

	/* Print results */
	print_results();

	return ret;
}
