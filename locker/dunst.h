#ifndef __DUNST_H
#define __DUNST_H

#include <signal.h>

#define dunst_pause()		dunst_kill(SIGUSR1)
#define dunst_resume()		dunst_kill(SIGUSR2)

void dunst_kill(int signum);

#endif /* __DUNST_H */

