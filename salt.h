#ifndef __CGIPAFSALTH
#define __CGIPAFSLATH
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#include "md5crypt.h"
char * md5_seed(void);
char * std_seed(void);
#endif
