#include <stdio.h>
#include <time.h>

#define MIN_INT(t)				(-(1L << ((sizeof(t) * 8) - 1)))
#define MAX_INT(t)				((1L << ((sizeof(t) * 8) - 1)) - 1L)
#define MAX_UINT(t)				((1UL << (sizeof(t) * 8)) - 1UL)

#define PRINT(t)					\
	do {						\
		printf("%s -- min: %ld, max: %ld\n",	\
			#t, MIN_INT(t), MAX_INT(t));	\
	} while (0)

#define UPRINT(t)					\
	do {						\
		printf("%s -- min: 0, max: %lu\n",	\
			#t, MAX_UINT(t));		\
	} while (0)

int main(void)
{
	PRINT(signed char);
	PRINT(short);
	PRINT(int);
	PRINT(long);
	PRINT(long long);
	PRINT(time_t);

	UPRINT(unsigned char);
	UPRINT(unsigned short);
	UPRINT(unsigned int);
	UPRINT(unsigned long);
	UPRINT(unsigned long long);
	return 0;
}
