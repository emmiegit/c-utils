#include <stdio.h>

int main(int argc, const char *argv[])
{
	int i;

	printf("argc: %d\n", argc);
	for (i = 0; i < argc; i++)
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	printf("argv[%d] = %p\n", i, (void *)argv[i]);
	return 0;
}
