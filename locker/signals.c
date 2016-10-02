#include <signal.h>

#include "main.h"
#include "signals.h"

static void handle_signal(int signum)
{
	switch (signum) {
	case SIGTERM:
	case SIGINT:
	case SIGHUP:
		cleanup();
		break;
	}
}

void signals_init(void)
{
	signal(SIGTERM, handle_signal);
	signal(SIGINT, handle_signal);
	signal(SIGHUP, handle_signal);
}

