#define _XOPEN_SOURCE 500

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ISNAN(x)	((x) != (x))
#define ISINF(x)	(((x) == (1.0/0.0)) || ((x) == (-1.0/0.0)))

static const char *argv0;
static pid_t main_pid;

static void usage_and_exit(void)
{
	fprintf(stderr, "usage: %s [-n DELAY] [--] COMMAND ARGS...\n", argv0);
	exit(1);
}

static void on_sigchld(int signum)
{
	int wstatus, ret;

	assert(signum == SIGCHLD);

	ret = waitpid(main_pid, &wstatus, WNOHANG);
	if (ret < 0) {
		perror("unable to wait on child");
		exit(1);
	}

	if (ret == main_pid) {
		exit(WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : 0);
	}
}

static void get_delay(struct timespec *delay, const char *s)
{
	char *ptr;
	double n;

	n = strtod(s, &ptr);
	if (*ptr) {
		fprintf(stderr, "invalid delay: '%s'\n", s);
		usage_and_exit();
	}
	if (ISINF(n) || ISNAN(n) || n <= 0.0) {
		fprintf(stderr, "delay out of range: '%s'\n", s);
		usage_and_exit();
	}

	delay->tv_sec = (time_t)n;
	delay->tv_nsec = (n - delay->tv_sec) * 1e9;
}

static void spawn_sudo(void)
{
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		perror("error forking new process");
		exit(1);
	}

	if (!pid) {
		/* child */
		execlp("sudo", "sudo", "true", NULL);
		perror("error executing process");
		_exit(1);
	}
}

int main(int argc, char **argv)
{
	struct timespec delay;
	char **child_argv;
	int ch;

	delay.tv_sec = 60;
	delay.tv_nsec = 0;

	/* Parse options */
	argv0 = argv[0];
	while ((ch = getopt(argc, argv, "n:")) != -1) {
		switch (ch) {
		case 'n':
			get_delay(&delay, optarg);
			break;
		case '?':
			usage_and_exit();
			break;
		default:
			abort();
		}
	}

	if (optind == argc) {
		fputs("no command given\n", stderr);
		usage_and_exit();
	}
	child_argv = &argv[optind];

	if (signal(SIGCHLD, on_sigchld) == SIG_ERR) {
		perror("error setting SIGCHLD handler");
		return 1;
	}

	/* Spawn process */
	main_pid = fork();
	if (main_pid < 0) {
		perror("error forking new process");
		return 1;
	}

	if (main_pid) {
		/* parent */
		for (;;) {
			if (nanosleep(&delay, NULL)) {
				assert(errno != EINVAL);
			}

			spawn_sudo();

			if (kill(main_pid, 0)) {
				if (errno == ESRCH) {
					break;
				}
			}
		}
	} else {
		/* child */
		execvp(child_argv[0], child_argv);
		perror("error executing process");
		_exit(1);
	}

	return 0;
}
