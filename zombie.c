#define _XOPEN_SOURCE	500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_INT(t)		((1UL << (8 * sizeof(t) - 1UL)) - 1UL)

/*
 * Intentionally creates a zombie child process for testing.
 *
 * Usage: zombie [hold|quit]
 *
 * If "quit" is specified, then the parent exits immediately after the zombie
 * child is created. The setting "hold" means that the parent waits forever.
 * The default is "hold".
 */

int main(int argc, const char **argv)
{
	struct timespec ts;
	unsigned int exit_after = 0;
	pid_t pid;

	if (argc > 1) {
		if (!strcmp(argv[1], "quit") || !strcmp(argv[1], "exit")) {
			exit_after = 1;
		} else if (!strcmp(argv[1], "hold") || !strcmp(argv[1], "wait")) {
			exit_after = 0;
		} else {
			fprintf(stderr, "Invalid argument: %s\n", argv[1]);
			exit(1);
		}
	}

	/* Main */
	pid = fork();
	switch (pid) {
	case -1:
		/* Error */
		perror("Unable to fork()");
		exit(1);
		break;
	case 0:
		/* Child */
		exit(0);
		break;
	default:
		/* Parent */
		printf("%d\n", pid);

		if (exit_after) {
			/* Quit right away */
			exit(0);
		}

		/* Otherwise loop forever */
		ts.tv_sec = MAX_INT(time_t);
		ts.tv_nsec = 0;

		for (;;) {
			if (nanosleep(&ts, NULL)) {
				/* Error */
				exit(1);
			}
		}
	}

	/* We shouldn't get here */
	return -1;
}
