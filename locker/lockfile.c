#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include "constants.h"
#include "lockfile.h"

static char *lock_file;

void lockfile_create(void)
{
	struct stat statbuf;
	struct passwd *pwd = getpwuid(getuid());
	if (!pwd) {
		fprintf(stderr, "Unable to find username for uid %u.\n",
			(unsigned int)getuid());
		exit(EXIT_FAILURE);
	}

	lock_file = malloc(strlen(LOCK_FILE) + strlen(pwd->pw_name));
	if (!lock_file) {
		perror("Unable to allocate memory");
		exit(EXIT_FAILURE);
	}

	sprintf(lock_file, LOCK_FILE, pwd->pw_name);

	if (stat(lock_file, &statbuf)) {
		FILE *fh;

		if (errno != ENOENT) {
			fprintf(stderr, "Unable to stat \"%s\": %s.\n",
				lock_file, strerror(errno));
			exit(EXIT_FAILURE);
		}

		fh = fopen(lock_file, "w");
		if (!fh) {
			fprintf(stderr, "Unable to create lock file \"%s\": %s.\n",
				lock_file, strerror(errno));
			exit(EXIT_FAILURE);
		}

		fprintf(fh, "%u\n", (unsigned int)getpid());
		fclose(fh);
		return;
	}

	fprintf(stderr, "Lock file \"%s\" already exists.\n", lock_file);
	exit(EXIT_FAILURE);
}

void lockfile_delete(void)
{
	if (unlink(lock_file)) {
		fprintf(stderr, "Unable to delete lock file \"%s\": %s.\n",
			lock_file, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

