#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "lock.h"
#include "lockfile.h"
#include "display.h"
#include "dunst.h"
#include "main.h"
#include "signals.h"

int main()
{
	signals_init();
	lockfile_create();
	display_read();
	dunst_pause();
	lock_screen();
	cleanup();
	return 0;
}

void cleanup(void)
{
	lockfile_delete();
	dunst_resume();
}

#define LOCK_FILE			"/tmp/%s-lockscreen.lock"
