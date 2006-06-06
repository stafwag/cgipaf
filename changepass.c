/*
 * changepass.c
 *
 * Copyright (C) 2002 Staf Wagemakers Belgie/Belgium
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

#include "common.h"
#include "pass.h"
char txt_usage[]="Usage \"echo $USER:$PASSWORD\" |";
char *prgname;
char *arguments[]={"-h","--help","-e","--encrypted"};

/*
 * display usage message
 */

void usage() {
      fprintf(stderr,"%s %s\n",txt_usage,prgname);
}

int match_args(char *arg) {

}

int main (int argn,char **argv) 
{
   int i;
   char line[1024]="\0";
   char *pass;
   char *name;
   char *c;
   struct pw_info *pw;
   
   prgname=argv[0];              /* set prgname to the real program name */

   if(argn>1) {                  /* we've no arguments */

	   int i;
	   int hlpflag=0;
	   int encryptflag=0;
	   int nopamflag=0;
	   int md5flag=0;

	   char *longargs[]={"encrypted","md5","help","nopam",NULL};

	   for (i=1;i<argn;i++) {

		   char *shortarg=NULL;

		   if (strncmp("--",argv[i],2)==0) {

			   	char **ccp;
				int found=0;

			   	for(ccp=longargs;*ccp!=NULL;++ccp) {

					if (!strcmp(argv[i]+2,*ccp)) {
			   	
						shortarg=xmalloc(3*sizeof(char));
						shortarg[0]=0;
						strncat(shortarg,(argv[i]+1),2);
						argv[i]=shortarg;
						found=1;
						break;

					}

				}

				if (!found) {
						fprintf(stderr,"Unkown argument %s\n",argv[i]);
				        	hlpflag=1;

				}
		   }
	   }

	   if(!hlpflag) {

	   while ((i = getopt(argn, argv, "hemn")) != -1) {

		   switch(i) {

		   	case 'h':
					hlpflag=1;
					break;
		   	case 'e':
					encryptflag=1;
					break;
		   	case 'm':
					md5flag=1;
					break;
		   	case 'n':
					nopamflag=1;
					break;
		   	case '?':	
					hlpflag=1;
					break;
			default:
					hlpflag=1;
		   }



	   }

	   }

	   printf("hlpflag = %d\n",hlpflag);
	   printf("encryptflag = %d\n",encryptflag);
	   printf("nopamflag = %d\n",nopamflag);
	   printf("md5flag = %d\n",md5flag);

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
