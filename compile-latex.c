#define _XOPEN_SOURCE 500

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define USE_VFORK
#define COMPILE_COMMAND	"pdflatex"

static void call_command(const char *filename)
{
	pid_t pid;

#ifdef USE_VFORK
	pid = vfork();
#else
	pid = fork();
#endif /* USE_VFORK */
	if (pid < 0) {
		perror("Unable to fork to run compilation command");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {
		/* We are the child */
		execlp(COMPILE_COMMAND, filename, NULL);
		perror("Unable to exec to run compilation command");
		_exit(EXIT_FAILURE);
	} else {
		waitpid(pid, NULL, 0);
	}
}

static void read_event(const struct inotify_event *event, const char *argv[])
{
	const char *filename = argv[event->wd];
	time_t now = time(NULL);

	if (event->mask == IN_IGNORED) {
		return;
	}

	printf(">> %s", ctime(&now));

	if (event->mask == IN_MODIFY) {
		call_command(filename);
	} else {
		printf("\"%s\" was deleted.\n", filename);
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, const char *argv[])
{
	int inotify_fd;
	int i;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s filename...\n", argv[0]);
		return EXIT_FAILURE;
	}

	inotify_fd = inotify_init1(IN_CLOEXEC);
	if (inotify_fd < 0) {
		perror("Unable to initialize inotify");
		return EXIT_FAILURE;
	}

	for (i = 1; i < argc; i++) {
		int wd = inotify_add_watch(inotify_fd, argv[i],
				IN_MODIFY | IN_DELETE | IN_DELETE_SELF
				| IN_MOVE_SELF | IN_MOVED_FROM);
		if (wd < 0) {
			fprintf(stderr, "Unable to add watch for \"%s\": %s.\n",
				argv[i], strerror(errno));
			return EXIT_FAILURE;
		}
	}

	while (1) {
		char buf[sizeof(struct inotify_event)];
		ssize_t len;

		len = read(inotify_fd, buf, sizeof(buf));

		if (len < 0) {
			if (errno == EINTR) {
				return EXIT_SUCCESS;
			}

			perror("Unable to read from inotify");
			return EXIT_FAILURE;
		}

		read_event((struct inotify_event *)buf, argv);
	}

	return EXIT_SUCCESS;
}

