#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "display.h"

struct display_data display_data;

void display_read(void)
{
	Display *display;
	char *display_name;
	int i;

	display_name = getenv("DISPLAY");
	if (!display_name) {
		fprintf(stderr, "No display specified.\n");
		exit(EXIT_FAILURE);
	}

	display = XOpenDisplay(display_name);
	if (!display) {
		fprintf(stderr, "Cannot open display.\n");
		exit(EXIT_FAILURE);
	}

	display_data.count = XScreenCount(display);
	display_data.screens = malloc(sizeof(struct screen) * display_data.count);
	if (!display_data.screens) {
		perror("Unable to allocate screen array");
		XCloseDisplay(display);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < display_data.count; i++) {
		display_data.screens[i].width = XDisplayWidth(display, i);
		display_data.screens[i].height = XDisplayHeight(display, i);
	}

	XCloseDisplay(display);
}

