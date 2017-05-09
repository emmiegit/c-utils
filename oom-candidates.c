#define _XOPEN_SOURCE	500

#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct oom_result {
	pid_t pid;
	unsigned int score;
	char name[NAME_MAX];
};

static void read_exe(struct oom_result *res)
{
	char exe[PATH_MAX];
	char path[32];
	ssize_t ret;

	sprintf(path, "/proc/%d/exe", res->pid);
	ret = readlink(path, exe, sizeof(exe));
	if (ret < 0) {
		res->name[0] = '\0';
		return;
	}

	exe[ret] = '\0';
	strncpy(res->name, basename(exe), sizeof(res->name));
}

static int read_oom_score(struct oom_result *res)
{
	char path[32];
	char buf[32];
	ssize_t ret;
	int fd;

	sprintf(path, "/proc/%d/oom_score", res->pid);
	fd = open(path, 0);
	if (fd < 0) {
		fprintf(stderr, "Unable to open '%s': %s\n",
			path, strerror(errno));
		return -1;
	}
	ret = read(fd, buf, sizeof(buf));
	if (ret < 0) {
		fprintf(stderr, "Unable to read from '%s': %s\n",
			path, strerror(errno));
		close(fd);
		return -1;
	}

	buf[ret - 1] = '\0';
	res->score = atoi(buf);
	close(fd);
	return 0;
}

static void print_result(const struct oom_result *res)
{
	printf("%u - %d %s\n", res->score, res->pid, res->name);
}

int main()
{
	const struct dirent *dirent;
	struct oom_result worst, tmp;
	DIR *dh;

	dh = opendir("/proc");
	if (!dh) {
		perror("Unable to open procfs");
		return 1;
	}

	worst.score = 0;
	while ((dirent = readdir(dh)) != NULL) {
		tmp.pid = atoi(dirent->d_name);
		if (!tmp.pid || read_oom_score(&tmp))
			continue;
		if (tmp.score > worst.score) {
			read_exe(&tmp);
			memcpy(&worst, &tmp, sizeof(struct oom_result));
		}
	}
	closedir(dh);
	print_result(&worst);
	return 0;
}
