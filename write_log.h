/*
 * write_logs.h					(c) 2001 Staf Wagemakers
 */

#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#ifndef LOG_AUTHPRIV
#define LOG_AUTHPRIV LOG_AUTH
#endif

int enable_log(int s);
void write_log(int facility, int priority, char *fmt, ...);
unsigned set_loglevel(int l);
