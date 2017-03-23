#include <stdlib.h>
#include <stdio.h>

#define BIT_8(x)	(x)
#define BIT_10(x)	((x) | 0x80)
#define BIT_110(x)	((x) | 0xc0)
#define BIT_1110(x)	((x) | 0xe0)
#define BIT_11110(x)	((x) | 0xf0)

int main(int argc, const char *argv[])
{
	unsigned int cp, max_cp;
	char i, j, k, l;

	/* Parse argument */
	if (argc == 1) {
		max_cp = 0x10ffff;
	} else {
		long val;
		char *ptr;

		val = strtol(argv[1], &ptr, 0);
		if (!*argv[1] || *ptr) {
			fprintf(stderr, "%s: invalid numeric value: '%s'\n",
				argv[0], argv[1]);
			return 1;
		} else if (val < 0) {
			fprintf(stderr, "%s: value cannot be negative: '%s'\n",
				argv[0], argv[1]);
			return 1;
		} else if (val > 0x10ffff) {
			val = 0x10ffff;
		}
		max_cp = val;
	}

	/* U+0000 to U+007F */
	cp = 0;
	for (i = 0; i < 0x7f; i++) {
		if (cp++ > max_cp)
			return 0;
		printf("U+%04X  '%c'\n", cp, BIT_8(i));
	}

	/* U+0080 to U+07FF */
	for (i = 0; i < 0x1f; i++) {
		for (j = 0; j < 0x3f; j++) {
			if (cp++ > max_cp)
				return 0;
			printf("U+%04X  '%c%c'\n",
				cp, BIT_110(i), BIT_10(j));
		}
	}

	/* U+0800 to U+FFFF */
	for (i = 0; i < 0x0f; i++) {
		for (j = 0; j < 0x3f; j++) {
			for (k = 0; k < 0x3f; k++) {
				if (cp++ > max_cp)
					return 0;
				printf("U+%04X  '%c%c%c'\n",
					cp,
					BIT_1110(i),
					BIT_10(j),
					BIT_10(k));
			}
		}
	}

	/* U+10000 + U+10FFFF */
	for (i = 0; i < 0x7; i++) {
		for (j = 0; j < 0x3f; j++) {
			for (k = 0; k < 0x3f; k++) {
				for (l = 0; l < 0x3f; l++) {
					if (cp++ > max_cp)
						return 0;
					printf("U+%05X '%c%c%c%c'\n",
						cp,
						BIT_11110(i),
						BIT_10(j),
						BIT_10(k),
						BIT_10(l));
				}
			}
		}
	}

	return 0;
}
