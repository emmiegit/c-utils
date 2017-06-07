#include <stdio.h>
#include <string.h>

#define SIGN(x)		(((x) > 0) - ((x) < 0))

int main(int argc, const char *argv[])
{
	int cmp;

	if (argc != 3)
		return 128;

	cmp = SIGN(strcmp(argv[1], argv[2]));
	printf("%d\n", cmp);
	return !!cmp;
}
