#include "config.h"
#include <stdlib.h>
#ifdef HAVE_SYS_SYSTEMINFO_H
#include <sys/systeminfo.h>
#endif
#ifndef HAVE_UNSETENV
void unsetenv(char *env_name);
#endif
#ifndef HAVE_SETENV
void unsetenv(char *env_name);
#endif
#ifndef HAVE_GETDOMAINNAME
char *getdomainname(); 
#endif
#ifndef HAVE_GETHOSTNAME
char *gethostname();
#endif
