#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 16
#define SLEEP_DELAY 5
#define ever (;;)

int kill(pid_t process, int signal);

int main()
{
    /* Read pid from stdin */
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, stdin);
    const pid_t pid = atoi(buffer);

    /* Loop until pid exits */
    int ret;

    for ever {
        sleep(SLEEP_DELAY);

        ret = kill(pid, 0);

        if (ret < 0 && errno == ESRCH) {
            return 0;
        }
    }
}

