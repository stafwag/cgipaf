/*
 * searchdomain.c
 *
 * Copyright (C) 2001 Staf Wagemakers Belgie/Belgium
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
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
     strncat(domainname,buf,len-1);
     i=0;
   } 
return(i);
}
