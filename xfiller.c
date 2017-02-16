#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_X		0
#define DEFAULT_Y		0
#define DEFAULT_WIDTH		850
#define DEFAULT_HEIGHT		700

static void create_window(Display *dis, Atom *ptr)
{
	Window win;
	XTextProperty window_name;
	char title[64];
	int black_color;

	black_color = BlackPixel(dis, DefaultScreen(dis));
	win = XCreateSimpleWindow(dis,
				  DefaultRootWindow(dis),
				  DEFAULT_X,
				  DEFAULT_Y,
				  DEFAULT_WIDTH,
				  DEFAULT_HEIGHT,
				  0,
				  0,
				  black_color);
	if (!win) {
		fprintf(stderr, "Unable to create window.\n");
		exit(1);
	}

	*ptr = XInternAtom(dis, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dis, win, ptr, 1);
	XSelectInput(dis, win, KeyPressMask);
	XMapWindow(dis, win);

	sprintf(title, "xfiller - pid %d", getpid());
	window_name.value = (unsigned char *)title;
	window_name.encoding = XA_STRING;
	window_name.format = 8;
	window_name.nitems = strlen(title);

	XSetWMName(dis, win, &window_name);
}

int main(void)
{
	Display *dis;
	Atom wm_delete_msg;
	XEvent evt;

	dis = XOpenDisplay(NULL);;
	if (!dis) {
		fprintf(stderr, "Unable to open X display.\n");
		exit(1);
	}
	create_window(dis, &wm_delete_msg);
	for (;;) {
		XNextEvent(dis, &evt);
		if ((Atom)evt.xclient.data.l[0] == wm_delete_msg) {
			break;
		}
	}
	XCloseDisplay(dis);
	return 0;
}

