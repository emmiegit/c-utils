#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ROLL()			((rand() % dice) + 1)

static const char *argv0;
static unsigned int dice, count;

static void error(const char *arg)
{
	fprintf(stderr, "%s: invalid argument: %s\n",
		argv0, arg);
	exit(EXIT_FAILURE);
}

static void parse_double(const char *spec)
{
	size_t i;
	unsigned int val;

	val = 0;
	for (i = 0; spec[i]; i++) {
		if (tolower(spec[i]) == 'd')
			break;
		else if (!isdigit(spec[i]))
			error(spec);

		val *= 10;
		val += spec[i] - '0';
	}
	if (!spec[i]) {
		dice = val;
		return;
	} else {
		count = val;
		i++;
	}

	val = 0;
	for (; spec[i]; i++) {
		if (!isdigit(spec[i]))
			error(spec);

		val *= 10;
		val += spec[i] - '0';
	}
	dice = val;
}

static void parse_int(const char *spec, unsigned int *val)
{
	const char *str;
	size_t i;

	if (tolower(spec[0]) == 'd')
		str = spec + 1;
	else
		str = spec;

	*val = 0;
	for (i = 0; str[i]; i++) {
		if (!isdigit(str[i]))
			error(spec);

		*val *= 10;
		*val += str[i] - '0';
	}
}

static void parse_diceroll(const char *spec)
{
	if (isdigit(spec[0]))
		parse_double(spec);
	else if (tolower(spec[0]) == 'd')
		parse_int(spec, &dice);
	else
		error(spec);
}

/* Usage: roll [[count] "d" dice-size] */
int main(int argc, const char *argv[])
{
	unsigned int sum;

	/* Initialization */
	srand(time(NULL));

	/* Parse arguments */
	argv0 = argv[0];
	count = 1;
	switch (argc) {
	case 1:
		dice = 6;
		break;
	case 2:
		parse_diceroll(argv[1]);
		break;
	default:
		parse_int(argv[1], &count);
		parse_int(argv[2], &dice);
	}

	/* Roll dice */
	sum = 0;
	if (count == 1) {
		sum = ROLL();
	} else {
		unsigned int i, x;

		sum = 0;
		for (i = 0; i < count; i++) {
			x = ROLL();
			sum += x;
			printf("Die %u: %u\n", i + 1, x);
		}
	}
	printf("Total: %u\n", sum);
	return 0;
}
