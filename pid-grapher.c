#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define UNUSED(x)	((void)(x))

/* 10 minutes */
#define SAMPLE_TIME	(10 * 60)

static const char *argv0;
static FILE *fh;

static void write_pid(void)
{
	pid_t pid;

	if ((pid = vfork()) < 0) {
		fprintf(stderr, "%s: unable to fork: %s\n",
			argv0, strerror(errno));
		exit(1);
	}
	if (pid == 0) {
		_exit(0);
	}
	waitpid(pid, NULL, 0);
	fprintf(fh, "%ld %d\n", time(NULL), pid);
}

static void write_pid2(int signum)
{
	UNUSED(signum);

	write_pid();
	fflush(fh);
}

static void setup(const char *path)
{
	fh = fopen(path, "a");
	if (!fh) {
		fprintf(stderr, "%s: %s: %s\n",
			argv0, path, strerror(errno));
		exit(1);
	}
	putc('-', fh);
	putc('\n', fh);
}

static void cleanup(int signum)
{
	UNUSED(signum);

	if (fclose(fh)) {
		perror("unable to close");
	}
	exit(0);
}

int main(int argc, const char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s log-file\n", argv[0]);
		return 1;
	}
	argv0 = argv[0];
	setup(argv[1]);
	signal(SIGTERM, cleanup);
	signal(SIGINT,  cleanup);
	signal(SIGABRT, cleanup);
	signal(SIGHUP,  write_pid2);
	for (;;) {
		write_pid();
		sleep(SAMPLE_TIME);
	}
	return 0;
}
