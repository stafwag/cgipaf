/*
 * searchdomain.c				(c) 2001   Staf Wagemakers
 */
#include "configfile.h"
int searchdomain(char *domainname,size_t len) {
FILE *fp;
char *buf=NULL;
int i=-1;
#ifndef HAVE_GETDOMAINNAME
#ifdef  HAVE_SYSTEMINFO_H
#ifdef  HAVE_SYSINFO
#define HAVE_GETDOMAINNAME
#endif
#endif
#endif
   
#ifdef HAVE_GETDOMAINNAME
   i=getdomainname(domainname,len-1);
#endif
   if (i==0) return(0);
   i=-1;
   if((fp=fopen("/etc/resolv.conf","r"))!=NULL) {
      buf=get_config_item(fp,"domain");
      if (buf==NULL) {
	 buf=get_config_item(fp,"search");
      }
   fclose(fp);
   }
   domainname[0]='\0';
   if(buf!=NULL) {
	   fprintf(stderr,"buf is not null");
     strncat(domainname,buf,len-1);
     i=0;
   } 
return(i);
}
