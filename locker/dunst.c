#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>

#include "dunst.h"

void dunst_kill(int signum)
{
	DIR *dirh;
	struct dirent *dirent;

	dirh = opendir("/proc");
	if (!dirh) {
		perror("Unable to open /proc");
		return;
	}

	while ((dirent = readdir(dirh)) != NULL) {
		char buf[256];
		int fd, pid;

		if (!isdigit(dirent->d_name[0])) {
			continue;
		}

		pid = atoi(dirent->d_name);
		if (pid == 0) {
			continue;
		}

		sprintf(buf, "/proc/%s/cmdline", dirent->d_name);

		fd = open (buf, O_RDONLY);
		if (fd < 0) {
			continue;
		}

		read(fd, buf, sizeof(buf));

		if (strstr(buf, "dunst")) {
			kill(pid, signum);
		}

		close(fd);
	}

	closedir(dirh);
}

