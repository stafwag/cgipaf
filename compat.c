#include "config.h"
#include <stdlib.h>
#ifdef HAVE_SYS_SYSTEMINFO_H
#include <sys/systeminfo.h>
#endif
void unsetenv(char *env_name) {
  extern char **environ;
  char **cc;
  int l;
  l=strlen(env_name);
  for (cc=environ;*cc!=NULL;cc++) {
    if (strncmp(env_name,*cc,l)==0 && ((*cc)[l]=='='||(*cc)[l]=='\0')) break;
  }
  for (; *cc != NULL; cc++) *cc=cc[1];
}

#ifndef HAVE_GETDOMAINNAME
char *getdomainname() {
char *domain=(char *) xmalloc(100);
sysinfo(SI_SRPC_DOMAIN,domain,100);
domain=(char *)xrealloc(domain,strlen(domain)+1);
return(domain);
}
#endif

#ifndef HAVE_GETHOSTNAME
char *gethostname() {
char hostname=(char *) xmalloc(100);
sysinfo(SI_HOSTNAME,hostname,100);
hostname=(char *)xrealloc(hostname,strlen(hostname)+1);
return(hostname);
}
#endif
