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
#define DEFAULT_DIRECTORY	"."
#define COMPILE_COMMAND		"pdflatex"

static void call_command(char *filename)
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
		execlp(COMPILE_COMMAND, COMPILE_COMMAND, filename, (char *)NULL);
		perror("Unable to exec to run compilation command");
		_exit(EXIT_FAILURE);
	} else {
		/* We are the parent */
		waitpid(pid, NULL, 0);
	}
}

static void read_event(const struct inotify_event *event)
{
	time_t now = time(NULL);

	if (event->mask == IN_IGNORED) {
		return;
	}

	/* vim leaves a file called "4913" for some reason when writing */
	if (!strcmp(event->name, "4913")) {
		return;
	}

	printf(">> %s", ctime(&now));

	if (event->mask == IN_CREATE) {
		call_command((char *)event->name);
	} else {
		puts("Directory was deleted or moved.");
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, const char *argv[])
{
	const char *directory = DEFAULT_DIRECTORY;
	int inotify_fd;
	int wd, ret;

	if (argc > 1) {
		directory = argv[1];
	}

	ret = chdir(directory);
	if (ret) {
		fprintf(stderr, "Unable to change directory to \"%s\": %s.\n",
			directory, strerror(errno));
		return EXIT_FAILURE;
	}

	inotify_fd = inotify_init1(IN_CLOEXEC);
	if (inotify_fd < 0) {
		perror("Unable to initialize inotify");
		return EXIT_FAILURE;
	}

	wd = inotify_add_watch(inotify_fd, directory, IN_CREATE | IN_DELETE_SELF | IN_MOVE_SELF);
	if (wd < 0) {
		fprintf(stderr, "Unable to add watch for \"%s\": %s.\n", directory, strerror(errno));
		return EXIT_FAILURE;
	}

	while (1) {
		char buf[sizeof(struct inotify_event) + NAME_MAX + 1];
		ssize_t len;

		len = read(inotify_fd, buf, sizeof(buf));

		if (len < 0) {
			if (errno == EINTR) {
				return EXIT_SUCCESS;
			}

			perror("Unable to read from inotify");
			return EXIT_FAILURE;
		}

		read_event((struct inotify_event *)buf);
	}

	return EXIT_SUCCESS;
}

