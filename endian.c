#include <stdio.h>
#include <string.h>

#define BIG_ENDIAN(u)		((u).c[0] != 1)
#define SHELL_BOOL(x)		(!(x))
#define UNUSED(x)		((void)(x))

int main(int argc, const char *argv[])
{
	union {
		int n;
		char c[sizeof(int)];
	} u;

	UNUSED(argc);

	u.n = 1;
	if (!strcmp(argv[0], "isbe")) {
		return SHELL_BOOL(BIG_ENDIAN(u));
	} else if (!strcmp(argv[0], "isle")) {
		return SHELL_BOOL(!BIG_ENDIAN(u));
	} else {
		puts(BIG_ENDIAN(u) ? "big endian" : "little endian");
	}
	return 0;
}
