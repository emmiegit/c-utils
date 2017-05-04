/*
 * pdf.c
 *
 * latex-autocompile - Automatically recompile LaTeX sources and
 *                     refresh the MuPDF viewer
 * Copyright (c) 2016-2017 Ammon Smith
 *
 * latex-autocompile is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * latex-autocompile is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with latex-autocompile.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "pdf.h"
#include "util.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_t thread;
static char *filename;
static pid_t pid;
static int running;

/* Locks */

static void lock(void)
{
	if (pthread_mutex_lock(&mutex))
		die("Unable to lock mutex");
}

static void unlock(void)
{
	if (pthread_mutex_unlock(&mutex))
		die("Unable to unlock mutex");
}

static void cond_wait(void)
{
	if (pthread_cond_wait(&cond, &mutex))
		die("Unable to wait on condition");
}

static void cond_broadcast(void)
{
	if (pthread_cond_broadcast(&cond))
		die("Unable to broadcast on condition");
}

/* Utilities */

void reopen(void)
{
	if (pid > 0) {
		if (kill(pid, SIGTERM))
			perror("Unable to send SIGTERM");
	}

	if ((pid = FORK()) < 0)
		die("Unable to fork");
	if (pid == 0) {
		/* We are the child */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		execlp(VIEW_COMMAND, VIEW_COMMAND, filename, (char *)NULL);
		_exit(-1);
	}
}

void refresh(void)
{
	if (pid > 0) {
		if (kill(pid, SIGHUP))
			perror("Unable to send SIGHUP");
	}
}

static void _child_(int signum, siginfo_t *inf, void *ucontext)
{
	UNUSED(signum);
	UNUSED(ucontext);

	assert(signum == SIGCHLD);

	if (inf->si_pid == pid)
		pid = 0;
	if (waitpid(inf->si_pid, NULL, 0) < 0) {
		if (errno != ECHILD)
			die("Unable to wait");
	}
}

static void *_thread_(void *arg)
{
	char *last_filename;

	UNUSED(arg);

	last_filename = NULL;
	while (running) {
		lock();
		cond_wait();

		/*
		 * These pointers should be identical,
		 * so "==" is intentional.
		 */
		if (!pid || filename != last_filename) {
			reopen();
			last_filename = filename;
		} else {
			refresh();
		}
		unlock();
	}
	return NULL;
}

/* Externals */

void pdf_init(void)
{
	struct sigaction act;

	running = 1;
	if (pthread_create(&thread, NULL, _thread_, NULL))
		die("Unable to start PDF thread");

	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = _child_;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGCHLD, &act, NULL))
		die("Unable to set SIGCHLD action");
}

void pdf_cleanup(void)
{
	running = 0;
	if (pthread_join(thread, NULL))
		die("Unable to join PDF thread");
	free(filename);
}

void pdf_trigger(const char *name)
{
	free(filename);
	filename = replace_ext(name, PDF_EXTENSION, PDF_EXTENSION_LEN);
	cond_broadcast();
}
