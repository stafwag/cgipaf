/*
 * createcookie.c				   (c) 2001 Staf Wagemakers
 */
#include "createcookie.h"
#define  MAX_COOKIE_SIZE 60
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
}
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
snprintf(cookie+strlen(cookie),10,"%d",time(0));
return(cookie);
}
