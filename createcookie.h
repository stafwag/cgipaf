/*
 * createcookie.h			         (c)2001  Staf Wagemakers
 */

#include "xmalloc.h"
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif

char *create_cookie();
