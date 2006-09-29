/*
 * changepass.c
 *
 * Copyright (C) 2002,2006 Staf Wagemakers Belgie/Belgium
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

char txt_usage[]="[OPTION]\n\noptions:\n\n   -h,--help\tprint this help\n   -n,--nopam\tdon't use pam\n   -p,--pam\tuse pam (default)\n   -e,--encrypt\tpassword is already encrypted, this option will disable pam\n   -m,--md5\tuse md5 encryption, this option will disable pam\n\n";

char *prgname;

/*
 * display usage message
 */

void usage() {
      fprintf(stderr,"usage: %s %s\n",prgname,txt_usage);
}

struct  pw_info * get_pw_info (char *name,int pamflag) {

   	struct pw_info *pw;

	#ifdef _WITHPAM

   	if (pamflag) {

	   	pw=get_pw(name);

   	}
   	else {
	   	pw=get_pw_nopam(name);

   	}

	#else

   		pw=get_pw(name);

	#endif

	return(pw);

}


int main (int argn,char **argv) 
{
   int i;
   char line[1024]="\0";
   char *pass;
   char **passwords=NULL;
   char *name;
   char **names=NULL;
   char *c;
   struct pw_info *pw;
   int hlpflag=0;
   int encryptflag=0;
   int nopamflag=0;
   int pamflag=0;
   int md5flag=0;
   int linecounter=0;
   int number_of_lines=0;
   int error=0;
   int pass_updated=0;
   
   prgname=argv[0];              /* set prgname to the real program name */

   if(argn>1) {                  /* we've no arguments */

	   int i;
	   char *longargs[]={"encrypted","md5","help","nopam","pam",NULL};

	   /*
	    * convert long opts to shorts opt
	    */

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

	   	while ((i = getopt(argn, argv, "hpemn")) != -1) {

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
		   		case 'p':
						pamflag=1;
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
	   printf("pamflag = %d\n",pamflag);
	   printf("nopamflag = %d\n",nopamflag);
	   printf("md5flag = %d\n",md5flag);

	   if (pamflag && nopamflag) {

		   fprintf(stderr,"Can't enable pam and nopam.\n\n");

		   hlpflag=1;

	   }
	   else {

		   if ( pamflag && ( md5flag || encryptflag ) ) {

			   fprintf(stderr,"--md5 and --encrypted are invalid with forced pam.\n\n");

			   hlpflag=1;

		   }
		   else {
		   	if ( md5flag || encryptflag ) {

				nopamflag=1;

				if (md5flag && encryptflag) {

					fprintf(stderr,"Can't use --md5 and --encrypted at the same time\n\n");
					hlpflag=1;

				}

			}

		   }

	   }

	   if(hlpflag) {

	   	usage();
	   	exit(1);

	   }

   }
   
#ifdef _WITHPAM

  if (!nopamflag) {

	   pamflag=1;

  }

#ifdef NO_CHPASS_CRYPT

  else {

	  fprintf(stderr,"%s is compiled without --nopam support\n",prgname);
	  exit(1);

  }

#endif

   
   set_pam_service("passwd");      /* set PAM service name */
   set_pam_chauth_flag(0);

#else

	if (pamflag) {

		fprintf(stderr,"%s is compiled without --pam support\n",prgname);
		exit(1);


	}
	else {

		pamflag=0;
		nopamflag=1;

	}
   
#endif

	/*
	 * parse stdin line by line
	 */

	linecounter=0;

	for (;;) {

   		if (fgets(line,1023,stdin)==NULL) break;          /* read the data from stdin */

   			if(strlen(line)==0) {

				if (linecounter == 0 ) {

      					usage();                      /* oops, no data */
      					exit(1);

				}else {
					continue;
				}

   		}

		/* 
		 * detect a buffer overrun
		 */
   
   		if(line[strlen(line)-1]!='\n') {

      			fprintf(stderr,"\nToo much data...\n");  /* buffer too small */
      			exit(1);

   		}


		/*
		 * chop
		 */

   		line[strlen(line)-1]='\0';

		/*
		 * Ignore empty lines
		 */

		if (strlen(line)==0) {

			continue;

		}

		/*
		 * check on password seperator
		 */
   
   		if((c=strstr(line,":"))==NULL) {

      			fprintf(stderr,"\nInput error no \":\" in input on line: %d\n\n",linecounter+1);
      			exit(1);

   		}
   
   		/* copy the username into name */
   
   		name=(char *) xmalloc((c-line)+1);
   		strncpy(name,line,(c-line));
   		name[c-line]='\0';
		names=(char **) xrealloc(names,(linecounter+2)*sizeof(char **));
		names[linecounter]=name;
		names[linecounter+1]=NULL;
   
   		/* copy the password into pass */
   
   		pass=(char *) xmalloc(strlen(c));
   		strncpy(pass,c+1,strlen(c)-1);
   		pass[strlen(c)]='\0';
		passwords=(char **) xrealloc(passwords,(linecounter+2)*sizeof(char **));
		passwords[linecounter]=pass;
		passwords[linecounter+1]=NULL;

		++linecounter;

	}

	number_of_lines=linecounter;

	/*
	 * loop through the user and vrfy that we can get the password info
	 */

	for (linecounter=0;linecounter<number_of_lines;++linecounter) {

		fprintf(stderr,"%s %s\n",names[linecounter],passwords[linecounter]);

		name=names[linecounter];
		pass=passwords[linecounter];

		pw=get_pw_info(name,pamflag);


   		if (!pw) {

      			fprintf(stderr,"\nFailed to get password info for user: %s on line: %d\n\n",name,linecounter+1); /* get_pw() failed */
      			exit(1);

   		};

	}

	/*
	 * walk again to update the passwords
	 */

	for (linecounter=0;linecounter<number_of_lines;++linecounter) {

		fprintf(stderr,"%s %s\n",names[linecounter],passwords[linecounter]);

		name=names[linecounter];
		pass=passwords[linecounter];

		pw=get_pw_info(name,pamflag);


   		if (!pw) {

      			fprintf(stderr,"\nFailed to get password info for user: %s on line: %d\n\n",name,linecounter+1); /* get_pw() failed */
			error=1;
      			continue;

   		};

#ifdef _WITHPAM

   		if (pamflag) {

			i=chpw(pw,pass);

   		}
   		else {

	   		int mode=0;

	   		if (md5flag) {

		   		mode=1;

	   		}
	   		else {

		   		if(encryptflag) {

			   	mode=2;

		   		}
	   		}

	   		i=chpw_nopam(pw,pass,mode);

   		}

#else

   		i=chpw(pw,pass);

#endif

   		if (i!=PASS_SUCCESS) {

      			/* chpw() failed */
      			fprintf(stderr,"\nCan't update password for user %s on line %d \n\n",name,linecounter+1);
#ifdef _WITHPAM

      			if (pamflag) {

      				puts(pam_strerror(pw->pamh,i));

      			}
#endif

			error=1;

   		}
		else {

			pass_updated++;

		}

	}

   	/* password updated */

	if (error) {

		fprintf(stderr,"\nSorry, couldn't update all passwords\n");

	}
   
   	fprintf(stderr,"%d of %d passwords updated...\n\n",pass_updated,number_of_lines);
   	exit(error);

}
