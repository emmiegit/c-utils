#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#include <sched.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_X		0
#define DEFAULT_Y		0
#define DEFAULT_WIDTH		850
#define DEFAULT_HEIGHT		700

static void create_window(Display *display, Atom *ptr)
{
	Window win;
	XTextProperty window_name;
	XVisualInfo vinfo;
	XSetWindowAttributes attr;
	char title[64];

	XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
	attr.colormap = XCreateColormap(display,
					DefaultRootWindow(display),
					vinfo.visual,
					AllocNone);
	attr.border_pixel = 0;
	attr.background_pixel = 0;

	win = XCreateWindow(display,
			    DefaultRootWindow(display),
			    DEFAULT_X,
			    DEFAULT_Y,
			    DEFAULT_WIDTH,
			    DEFAULT_HEIGHT,
			    0,
			    vinfo.depth,
			    InputOutput,
			    vinfo.visual,
			    CWColormap | CWBorderPixel | CWBackPixel, &attr);
	if (!win) {
		fprintf(stderr, "Unable to create window.\n");
		exit(1);
	}

	*ptr = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, win, ptr, 1);
	XSelectInput(display, win, KeyPressMask);
	XMapWindow(display, win);

	sprintf(title, "[%d] xfiller", getpid());
	window_name.value = (unsigned char *)title;
	window_name.encoding = XA_STRING;
	window_name.format = 8;
	window_name.nitems = strlen(title);
	XSetWMName(display, win, &window_name);
}

int main(void)
{
	Display *dis;
	Atom wm_delete_msg;
	XEvent evt;

	dis = XOpenDisplay(NULL);;
	if (!dis) {
		fputs("Unable to open X display.\n", stderr);
		exit(1);
	}
	create_window(dis, &wm_delete_msg);
	for (;;) {
		sched_yield();
		XNextEvent(dis, &evt);
		if ((Atom)evt.xclient.data.l[0] == wm_delete_msg) {
			break;
		}
	}
	XCloseDisplay(dis);
	return 0;
}
