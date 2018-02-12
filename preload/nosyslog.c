#include <stdarg.h>

#define UNUSED(x)	((void)(x))

void openlog(const char *ident, int option, int facility)
{
	UNUSED(ident);
	UNUSED(option);
	UNUSED(facility);
}

void syslog(int priority, const char *format, ...)
{
	UNUSED(priority);
	UNUSED(format);
}

void vsyslog(int priority, const char *format, va_list ap)
{
	UNUSED(priority);
	UNUSED(format);
	UNUSED(ap);
}

void closelog(void)
{
}
