#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int interactive(void)
{
	char buf[4096];
	ssize_t ret, total;

	total = 0;
	do {
		ret = read(STDIN_FILENO, buf, sizeof(buf));
		if (ret < 0) {
			perror("unable to read from input");
			return 1;
		}
		total += ret;
	} while (ret);
	printf("%ld\n", total);
	return 0;
}

int main(int argc, const char *argv[])
{
	int i;

	if (argc == 1)
		return interactive();

	for (i = 1; i < argc; i++)
		printf("%zu\n", strlen(argv[i]));
	return 0;
}
