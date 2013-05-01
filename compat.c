/*
 * compat.c - emulate functions that aren't available on some Unices -
 *
 * Copyright (C) 2001    Staf Wagemakers Belgie/Belgium
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "compat.h"
#include "xmalloc.h"
#include <stdlib.h>
#ifdef HAVE_SYS_SYSTEMINFO_H
#include <sys/systeminfo.h>
#endif

#ifndef HAVE_UNSETENV
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
#endif

#ifndef HAVE_SETENV

int setenv(const char *name, const char *value, int overwrite) {

	extern char **environ;
	char **cc;
	char *env_str;
	int env_size=0;
	static int is_my_env=0;
	int name_len=strlen(name);

	/*
	 * create the string to insert in the environ table
	 */

	env_str=xmalloc(name_len+strlen(value)+2);
	strcpy(env_str,name);
	strcat(env_str,"=");
	strcat(env_str,value);

	/*
	 * parse the environ table and find evriron length
	 * if the env name exists replace it if overwrite is true and exit
	 */

	for(cc=environ;*cc;cc++) {

		if(!strncmp(*cc,name,name_len)) {

			if(overwrite) 
				*cc=env_str;
			else 
				free(env_str);

			return 0;

		}

		env_size++;
	}

	/*
	 * do a new malloc/memcpy if is_my_env if false
	 * else realloc
	 */

	if (is_my_env) {
		environ=realloc(environ,sizeof(char*)*(env_size+2));
	}
	else {
		cc=malloc(sizeof(char*)*(env_size+2));
		memcpy(cc,environ,sizeof(char*)*env_size);
		environ=cc;
		is_my_env=1;
	}

	/*
	 * add env_str to the environ table
	 */

	environ[env_size]=env_str;
	environ[env_size+1]=NULL;

	return 0;

}

#endif

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
