/*
 * createcookie.c
 *
 * Copyright (C) 2001,2007   Staf Wagemakers Belgie/Belgium
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

#include "createcookie.h"
#define  MAX_COOKIE_SIZE 60
/*
 * create a valid random char
 */
int gen_random_char(int s)
{
int i;
srand(s);
i=rand();
i=i/(RAND_MAX/63);
if(i==';') i=60;
if(i=='=') i=62;
if(i<0) i=0;
if(i>63) i=63;
if(i<12) return (i+46);
if((i>=12)&&(i<38)) return (i+(65-12));
if(i>=38) return (i+(97-38));

return(i);

}
/*
 * create a random cookie string
 */
char *create_cookie() {
char *c,*cookie;
int i;
cookie=(char *)xmalloc(MAX_COOKIE_SIZE);
for (i=0;i<MAX_COOKIE_SIZE;i++) cookie[i]='\0';
srand(getpid());
for (i=0;i<6;i++) {
    cookie[i]=gen_random_char(time(0)-rand());
}
cookie[6]='.';
c=getenv("REMOTE_ADDR");
if (c!=NULL) strncat(cookie+7,c,15);
   else strncat(cookie+7,"000.000.000.000",15);
cookie[strlen(cookie)]='.';
for(i=0;i<10;i++) {
   cookie[strlen(cookie)]=gen_random_char(time(0)-rand());
}
cookie[strlen(cookie)]='.';
for(i=0;i<10;i++) {
   srand(rand());
   cookie[strlen(cookie)]=gen_random_char(time(0)-rand());
}
cookie[strlen(cookie)]='.';
snprintf(cookie+strlen(cookie),10,"%d",(int) time(0));
return(cookie);
}
