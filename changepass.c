/* 
 * changepass.c
 * (c)2002 Staf Wagemakers
 */

#include "pass.h"
#include <string.h>
char txt_usage[]="Usage \"echo $USER:$PASSWORD\" |";
char *prgname;

/*
 * display usage message
 */

void usage() {
      fprintf(stderr,"%s %s",txt_usage,prgname);
}

int main (int argn,char argv[]) 
{
   int i;
   char line[1024]="\0";
   char *pass;
   char *name;
   char *c;
   struct pw_info *pw;
   
   prgname=argv[0];              /* set prgname to the real program name */

   if(argn>1) {                  /* we've no arguments */
      usage();
      exit(1);
   }
   
#ifdef _WITHPAM
   
   set_pam_service("passwd");      /* set PAM service name */
   set_pam_chauth_flag(0);
   
#endif

   fgets(line,1023,stdin);          /* read the data from stdin */

   if(strlen(line)==0) {
      usage();                      /* oops, no data */
      exit(1);
   }
   
   if(line[strlen(line)-1]!='\n') {
      fprintf(stderr,"Too much data...\n");  /* buffer too small */
      exit(1);
   }

   line[strlen(line)-1]='\0';
   
   if((c=strstr(line,":"))==NULL) {
      usage();                      /* no ':' to seperate username and pw */
      exit(1);
   }
   
   /* copy the username into name */
   
   name=(char *) xmalloc((c-line)+1);
   strncpy(name,line,(c-line));
   name[c-line]='\0';
   
   /* copy the password into pass */
   
   pass=(char *) xmalloc(strlen(c));
   strncpy(pass,c+1,strlen(c)-1);
   pass[strlen(c)]='\0';

   if (!(pw=get_pw(name))) {
      fprintf(stderr,"User %s doesn't exists\n",name); /* get_pw() failed */
      exit(1);
   };

   if ((i=chpw(pw,pass))!=PASS_SUCCESS) {
      /* chpw() failed */
      fprintf(stderr,"Can't update password for user %s\n",name);
#ifdef _WITHPAM
      puts(pam_strerror(pw->pamh,i));
#endif 
      exit(1);
   }

   /* password updated */
   
   fprintf(stderr,"passwd updated...\n");
   exit(0);

}
