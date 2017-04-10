#include <stdio.h>

int main(void)
{
	union {
		int n;
		char c[sizeof(int)];
	} u;

	u.n = 1;
	if (u.c[0] == 1)
		puts("little endian");
	else
		puts("big endian");
	return 0;
}
