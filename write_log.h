#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
int enable_log(int s);
void write_log(int facility, int priority, char *fmt, ...);

