#define _XOPEN_SOURCE 500

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define DEFAULT_DIRECTORY	"."
#define COMPILE_COMMAND		"pdflatex"
#define VIEW_COMMAND		"mupdf"
#define SOURCE_EXTENSION	".tex"
#define SOURCE_EXTENSION_LEN	4
#define PDF_EXTENSION		".pdf"
#define PDF_EXTENSION_LEN	4

#define UNUSED(x)		((void)(x))

static pthread_mutex_t pdf_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t pdf_cond = PTHREAD_COND_INITIALIZER;
static char *pdf_filename;
static pid_t pdf_pid;

static int endswith(const char *str, const char *suffix, size_t suflen)
{
	size_t i, len;

	len = strlen(str);
	for (i = 0; i < suflen; i++) {
		if (str[i + len - suflen] != suffix[i]) {
			return 0;
		}
	}
	return 1;
}

static char *replace_ext(const char *str, const char *ext, size_t extlen)
{
	size_t i, len;
	char *new;

	len = strlen(str);
	new = malloc(len);
	if (!new) {
		perror("Unable to allocate memory");
		exit(1);
	}
	memcpy(new, str, len - extlen);
	for (i = 0; i < extlen; i++) {
		new[i + len - extlen] = ext[i];
	}
	new[len] = '\0';
	return new;
}

static int call_command(const char *filename)
{
	pid_t pid;
	int ret;

	if ((pid = vfork()) < 0) {
		perror("Unable to fork");
		exit(-1);
	}

	if (pid == 0) {
		/* We are the child */
		close(STDIN_FILENO);
		execlp(COMPILE_COMMAND, COMPILE_COMMAND, filename, (char *)NULL);
		_exit(-1);
	} else {
		waitpid(pid, &ret, 0);
	}
	return ret;
}

static void pdf_reopen(void)
{
	if (pdf_pid > 0) {
		if (kill(pdf_pid, SIGTERM)) {
			perror("Unable to send SIGTERM");
		}
	}
	if ((pdf_pid = vfork()) < 0) {
		perror("Unable to fork");
		exit(-1);
	}
	if (pdf_pid == 0) {
		/* We are the child */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		execlp(VIEW_COMMAND, VIEW_COMMAND, pdf_filename, (char *)NULL);
		_exit(-1);
	}
}

static void pdf_refresh(void)
{
	if (pdf_pid <= 0) {
		return;
	}
	if (kill(pdf_pid, SIGHUP)) {
		perror("Unable to send SIGHUP");
		return;
	}
}

static void pdf_cleanup(int signum, siginfo_t *inf, void *ucontext)
{
	pid_t ret;

	UNUSED(signum);
	UNUSED(ucontext);

	if (inf->si_pid == pdf_pid) {
		pdf_pid = 0;
	}

	ret = waitpid(inf->si_pid, NULL, 0);
	if (ret <= 0 && errno != ECHILD) {
		perror("Unable to wait");
	}
}

static void *pdf_main(void *arg)
{
	char *last_filename;

	UNUSED(arg);

	last_filename = NULL;
	for (;;) {
		if (pthread_mutex_lock(&pdf_mutex)) {
			exit(-1);
		}
		if (pthread_cond_wait(&pdf_cond, &pdf_mutex)) {
			exit(-1);
		}

		/*
		 * These pointers should be identical,
		 * so "==" is intentional.
		 */
		if (!pdf_pid || pdf_filename != last_filename) {
			pdf_reopen();
			last_filename = pdf_filename;
		} else {
			pdf_refresh();
		}
		if (pthread_mutex_unlock(&pdf_mutex)) {
			exit(-1);
		}

	}
	return NULL;
}

static void read_event(const struct inotify_event *event)
{
	const time_t now = time(NULL);

	if (event->mask == IN_IGNORED) {
		return;
	}
	if (!endswith(event->name, SOURCE_EXTENSION, SOURCE_EXTENSION_LEN)) {
		return;
	}

	printf(">> %s", ctime(&now));
	if (event->mask == IN_CREATE) {
		if (call_command(event->name)) {
			return;
		}
	} else {
		puts("Directory was deleted or moved.");
		exit(0);
	}

	free(pdf_filename);
	pdf_filename = replace_ext(event->name, PDF_EXTENSION, PDF_EXTENSION_LEN);
	if (pthread_cond_broadcast(&pdf_cond)) {
		exit(-1);
	}
}

void setup(const char *directory)
{
	struct sigaction act;

	if (chdir(directory)) {
		fprintf(stderr, "Unable to change directory to \"%s\": %s.\n",
			directory, strerror(errno));
		exit(EXIT_FAILURE);
	}

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = pdf_cleanup;
	if (sigaction(SIGCHLD, &act, NULL)) {
		perror("Unable to set SIGCHLD action");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, const char *argv[])
{
	const char *directory;
	pthread_t view_thread;
	int inotify_fd;
	int wd;

	directory = DEFAULT_DIRECTORY;
	if (argc > 1) {
		directory = argv[1];
	}
	setup(directory);
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

	if (pthread_create(&view_thread, NULL, pdf_main, NULL)) {
		fprintf(stderr, "Unable to spawn view thread: %s.\n", strerror(errno));
		return EXIT_FAILURE;
	}
	printf("Listening on \"%s\"\n", directory);
	for (;;) {
		char buf[sizeof(struct inotify_event) + PATH_MAX + 1];
		ssize_t len;

		len = read(inotify_fd, buf, sizeof(buf));
		if (len < 0) {
			if (errno == EINTR) {
				return 0;
			}
			perror("Unable to read from inotify");
			return EXIT_FAILURE;
		}
		read_event((struct inotify_event *)buf);
	}

	return 0;
}

