#ifndef __DISPLAY_H
#define __DISPLAY_H

#undef EXTERN
#ifndef __DISPLAY_EXTERN
# define __DISPLAY_EXTERN
# define EXTERN extern
#else
# define EXTERN
#endif /* __DISPLAY_EXTERN */

EXTERN struct display_data {
	int count;

	struct screen {
		int width;
		int height;
	} *screens;
} display_data;

void display_read(void);

#endif /* __DISPLAY_H */

