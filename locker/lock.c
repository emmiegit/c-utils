#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "constants.h"
#include "display.h"
#include "lock.h"
#include "main.h"

#define TEMPFILE_TEMPLATE				"/tmp/lockscreen-XXXXXX.png"
#define TEMPFILE_LENGTH					27

struct worker {
	char path[TEMPFILE_LENGTH];
	int fd;
	pid_t process;
};

static void loop_pipe(char ***cmd)
{
	int fd[2];
	pid_t pid;
	int fd_in = STDIN_FILENO;

	while (*cmd) {
		pipe(fd);
		pid = fork();

		if (pid < 0) {
			perror("Unable to fork");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			dup2(fd_in, STDIN_FILENO);
			if (*(cmd + 1)) {
				dup2(fd[1], STDOUT_FILENO);
			}

			close(fd[0]);
			execvp((*cmd)[0], *cmd);
			_exit(EXIT_FAILURE);
		} else {
			waitpid(pid, NULL, 0);
			close(fd[1]);
			fd_in = fd[0];
			cmd++;
		}
	}
}

static void spawn_worker(struct worker *worker)
{
	strcpy(worker->path, TEMPFILE_TEMPLATE);
	worker->fd = mkstemps(worker->path, 4);
	if (worker->fd < 0) {
		perror("Unable to make temporary file");
		cleanup();
		exit(EXIT_FAILURE);
	}



}

static void lock_screen_single(void)
{
	char *maim[] = {
		"maim",
		"--opengl",
		"--format",
		"png",
		"/dev/stdout",
		NULL
	};
	char *convert[] = {
		"convert",
		"/dev/stdin",
		"-scale",
		SMALL_SCALE,
		"-scale",
		LARGE_SCALE,
		"/dev/stdout",
		NULL
	};
	char *composite[] = {
		"composite",
		"-gravity",
		"Center",
		LOCK_IMAGE,
		"/dev/stdin",
		"/dev/stdout",
		NULL
	};
	char *i3lock[] = {
		"i3lock",
		"-i",
		"/dev/stdin",
		NULL
	};
	char ***cmd = malloc(sizeof(char **) * 5);
	if (!cmd) {
		perror("Unable to allocate cmdline array");
		exit(EXIT_FAILURE);
	}

	cmd[0] = maim;
	cmd[1] = convert;
	cmd[2] = composite;
	cmd[3] = i3lock;
	cmd[4] = NULL;

	loop_pipe(cmd);
	free(cmd);
}

static void lock_screen_multi(void)
{
	struct worker *workers;
	int i;

	workers = malloc(sizeof(struct worker) * display_data.count);
	if (!workers) {
		perror("Unable to allocate tempfile array");
		cleanup();
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < display_data.count; i++) {
		spawn_worker(&workers[i]);
	}

	for (i = 0; i < display_data.count; i++) {
		waitpid(workers[i].process, NULL, 0);
	}

	/* join */

	for (i = 0; i < display_data.count; i++) {
		close(workers[i].fd);
	}

	free(workers);
}

void lock_screen(void)
{
	if (1 || display_data.count == 1) {
		lock_screen_single();
	} else {
		lock_screen_multi();
	}
}

