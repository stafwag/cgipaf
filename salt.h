#ifndef __CGIPAFSALTH
#define __CGIPAFSLATH
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifndef FREEBSDHOST
#include "md5crypt.h"
#endif
char * md5_seed(void);
char * std_seed(void);
#endif
