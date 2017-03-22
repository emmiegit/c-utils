/*
 * pidloop.c
 * Idle while the given pid exists.
 *
 * Licensed under the Apache 2.0 License.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#define SLEEP_DELAY 5

int kill(pid_t process, int signal);

int main()
{
	/* Read pid from stdin */
	char buf[32];
	pid_t pid;

	fgets(buf, sizeof(buf), stdin);
	pid = atoi(buf);

	for (;;) {
		sleep(SLEEP_DELAY);
		if (kill(pid, 0) < 0 && errno == ESRCH)
			return 0;
	}
}

