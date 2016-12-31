#include <X11/Xlib.h>
#include <stdio.h>

#define DEFAULT_X		0
#define DEFAULT_Y		0
#define DEFAULT_WIDTH		850
#define DEFAULT_HEIGHT		700

int main(void)
{
	Display *dis;
	Window win;
	XEvent evt;
	Atom wm_delete_msg;
	int black_color;

	dis = XOpenDisplay(NULL);;
	if (!dis) {
		fprintf(stderr, "Unable to open X display.\n");
		return 1;
	}
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
		return 1;
	}
	wm_delete_msg = XInternAtom(dis, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dis, win, &wm_delete_msg, 1);
	XSelectInput(dis, win, KeyPressMask);
	XMapWindow(dis, win);

	for (;;) {
		XNextEvent(dis, &evt);
		if ((Atom)evt.xclient.data.l[0] == wm_delete_msg) {
			break;
		}
	}
	XCloseDisplay(dis);
	return 0;
}

