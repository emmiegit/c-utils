#define _XOPEN_SOURCE	500

#include <limits.h>
#include <time.h>

#define MAX_INT(t)		((1UL << (CHAR_BIT * sizeof(t) - 1UL)) - 1UL)

int main(void)
{
	struct timespec ts;

	ts.tv_sec = MAX_INT(time_t);
	ts.tv_nsec = 0;

	for (;;) {
		if (nanosleep(&ts, NULL))
			return 1;
	}
	return 0;
}
