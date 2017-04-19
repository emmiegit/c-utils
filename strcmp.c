#include <stdio.h>
#include <string.h>

#define SIGN(x)		(((x) > 0) - ((x) < 0))

int main(int argc, const char *argv[])
{
	if (argc != 3)
		return 128;
	return SIGN(strcmp(argv[1], argv[2]));
}
