#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static long *primes;
static size_t len;

static void fill_array(void)
{
	size_t i;

	for (i = 0; i < len; i++)
		primes[i] = i + 2;
}

static void sieve_eratosthenes(void)
{
	size_t n;

	for (n = 2; n < (len / 2) + 1; n++) {
		size_t i;

		for (i = n + 1; i < len; i++) {
			if (primes[i] % n == 0)
				primes[i] = -1;
		}
	}
}

static void print_primes(void)
{
	size_t i;

	for (i = 0; i < len; i++) {
		if (primes[i] > 0)
			printf("%ld, ", primes[i]);
	}
	putchar('\n');
}

static long sum_primes(void)
{
	size_t i;
	long sum;

	sum = 0;
	for (i = 0; i < len; i++) {
		if (primes[i] > 0)
			sum += primes[i];
	}
	return sum;
}

/* Usage: sum-primes [MAX-VALUE] */
int main(int argc, const char *argv[])
{
	long sum;
	size_t max;

	if (argc > 1) {
		max = atol(argv[1]);
		if (max <= 1) {
			fprintf(stderr, "%s: max-value must be an integer greater than 2, not '%s'\n",
				argv[0], argv[1]);
			return 1;
		}
	} else {
		max = 500;
	}

	len = max - 1;
	primes = malloc(len * sizeof(long));
	if (!primes) {
		fprintf(stderr, "%s: unable to allocate array: %s\n",
			argv[0], strerror(errno));
		return 1;
	}

	fill_array();
	sieve_eratosthenes();
	print_primes();
	sum = sum_primes();

	printf("Sum of all primes below %zu is %ld.\n",
		max, sum);

	free(primes);
	return 0;
}
