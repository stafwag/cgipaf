/*
 * changepass.c
 *
 * Copyright (C) 2002,2006,2007,2015 Staf Wagemakers Belgie/Belgium
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

#include "common.h"
#include "pass.h"
#include <libgen.h>
#include <errno.h>


char *prgname;

int isoptarg(char *optstr) {

	if(optstr==NULL) return 0;

	switch(*optstr) {

		case 0:
				return(0);
				break;
		case '-':	
				return(0);
				break;
		default:
				return(1);
				break;

	}

	return(0);

}

/*
 * display usage message
 */

void usage() {

	char txt_copyright[]="                        (GPL) 2002-2015 Belgie  Belgium\n                                                            Staf Wagemakers\n                                                  email: staf@wagemakers.be";

	char txt_usage000[]="[OPTION]\n\noptions:\n\n -h,--help\t\tprint this help\n -n,--nopam\t\tdon't use pam\n";
	char txt_usage001[]=" -p,--pam\t\tuse pam (default)\n";
	char txt_usage002[]=" -e,--encrypt\t\tpassword is already encrypted";
	char txt_usage003[]=" -m,--md5\t\tuse md5 algorithm";
	char txt_usage004[]=" -c,--crypt-method\tuse";
	char txt_usage005[]="crypt method";
	char txt_usage006[]=" -s,--sha-rounds\tnumber of sha2 rounds\n";
	char txt_usage007[]=" -v,--verbose\t\tenable verbose output\n\n";

#ifdef _WITHPAM
	char txt_disable_pam[]=", this option will disable pam\n";
#else
	char txt_disable_pam[]="\n";
#endif

	char * txt_supported_algo=string_array_to_str(xcrypt_supported_crypts());
	replace_char(txt_supported_algo,' ','|');

      	fprintf(stderr,"%s %s",prgname,(char *)CGIPAF_VERSION);
	fprintf(stderr,"%s\n\n",txt_copyright);


      	fprintf(stderr,"usage: %s %s",prgname,txt_usage000);
#ifdef _WITHPAM
	fprintf(stderr,"%s",txt_usage001);
#endif
	fprintf(stderr,"%s%s",txt_usage002,txt_disable_pam);
#ifdef MD5_CRYPT
	fprintf(stderr,"%s%s",txt_usage003,txt_disable_pam);
#endif
 	fprintf(stderr,"%s %s %s\n\t\t\t%s",txt_usage004,txt_supported_algo,txt_usage005,txt_disable_pam);
#ifdef MODERNCRYPT_SHA2 
 	fprintf(stderr,"%s",txt_usage006);
#endif
 	fprintf(stderr,"%s",txt_usage007);
	fprintf(stderr,"\n");
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
   int c_nopamflag=0;
   int nopamflag=0;
   int nopammode=-1;
   int pamflag=0;
   int md5flag=0;
   int algoflag=0;
   char *algorithme=NULL;
   char algomode=-1; 
   int sharoundsflag=0;
   char *sharounds_str=NULL;
   int sharounds=0;
   int verboseflag=0;
   int linecounter=0;
   int number_of_lines=0;
   int error=0;
   int pass_updated=0;
   
   prgname=basename(argv[0]);              /* set prgname to the real program name */

   if(argn>1) {                  /* we've no arguments */

	   int i;
	   char *longargs[]={"encrypted","md5","help","nopam","pam","verbose","crypt-method","--sha-rounds",NULL};

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
						fprintf(stderr,"%s: Unkown argument %s\n",prgname,argv[i]);
				        	hlpflag=1;

				}
		   }
	   }

	   if(!hlpflag) {

	   	while ((i = getopt(argn, argv, "hpemnvc:s:")) != -1) {

			switch(i) {

		   		case 'h':
						hlpflag=1;
						break;
		   		case 'e':
						if (encryptflag) {

							fprintf(stderr,"Sorry, -e can only be used ones\n\n");
							hlpflag=1;

						}

						encryptflag=1;
						nopammode=254;
						break;
		   		case 'm':
						if (md5flag) {

							fprintf(stderr,"Sorry, -m can only be used ones\n\n");
							hlpflag=1;

						}

						md5flag=1;
						nopammode=1;
						break;
		   		case 'n':
						if (c_nopamflag) {

							fprintf(stderr,"Sorry, -n can only be used ones\n\n");
							hlpflag=1;

						}

						c_nopamflag=1;
						nopamflag=1;
						break;
		   		case 'p':
						if (pamflag) {

							fprintf(stderr,"Sorry, -p can only be used ones\n\n");
							hlpflag=1;

						}

						pamflag=1;
						break;
				case 'v':
						if (verboseflag) {

							fprintf(stderr,"Sorry, -v can only be used ones\n\n");
							hlpflag=1;

						}

						verboseflag=1;
						break;
		   		case '?':	
						hlpflag=1;
						break;
				case 'c':
						if (algoflag) {

							fprintf(stderr,"Sorry, -c can only be used ones\n\n");
							hlpflag=1;

						}

						algoflag=1;

						if(!isoptarg(optarg)) {

							fprintf(stderr,"Sorry, option -c requires an argument.\n\n");
							hlpflag=1;
							break;

						}

						algorithme=optarg;

						algomode=cryptstr2int(algorithme);

						if(algomode==-1) {
							fprintf(stderr,"Sorry, unknown crypt type (%s).\n\n",null2str(algorithme));
							hlpflag=1;

						} else {

							if (is_crypt_id_supported(algomode)) {

								nopammode=algomode;

							} else {
								fprintf(stderr,"Sorry, unsupported crypt type name = (%s) crypt_id = (%d).\n\n",null2str(algorithme),algomode);
								hlpflag=1;


							}			

						}

						break;
				case 's':
						if (sharoundsflag) {

							fprintf(stderr,"Sorry, -s can only be used ones\n\n");
							hlpflag=1;

						}

   						sharoundsflag=1;

						if(!isoptarg(optarg)) {

							fprintf(stderr,"Sorry, option -s requires an argument.\n\n");
							hlpflag=1;
							break;

						}

						sharounds_str=optarg;

						if(!isstrdigit(sharounds_str)) {

							fprintf(stderr,"Sorry, option -s needs to be a digit string\n\n");
							hlpflag=1;
							break;

						}

						int i=sscanf(sharounds_str,"%d",&sharounds);

						if(i!=1) {

	   						fprintf(stderr,"sscanf failed (i=%d) is not 1, errno=%d errstr=%s",i,errno,null2str(strerror(errno)));
							exit(3);

						}

						char sharounds_test_buffer[1024];

						if (snprintf(sharounds_test_buffer,1023,"%d",sharounds)>=1023) {

							fprintf(stderr,"Invalid nummeric value length>1023\n");
							hlpflag=1;
							break;

						}

						if(strcmp(sharounds_test_buffer,sharounds_str)) {

							fprintf(stderr,"Invalid nummeric value\n");
							hlpflag=1;
							break;

						}

						break;

				default:
						hlpflag=1;
		   	}

	   	}

	   }

	   if ( verboseflag ) {
		
		fprintf(stderr,"DEBUG: option flags\n\n");

	   	fprintf(stderr,"hlpflag = %d\n",hlpflag);
	   	fprintf(stderr,"encryptflag = %d\n",encryptflag);
	   	fprintf(stderr,"pamflag = %d\n",pamflag);
	   	fprintf(stderr,"nopamflag = %d\n",nopamflag);
	   	fprintf(stderr,"nopammode = %d\n",nopammode);
	   	fprintf(stderr,"md5flag = %d\n",md5flag);
	   	fprintf(stderr,"algoflag = %d\n",algoflag);
	   	fprintf(stderr,"algorithme = %s\n",null2str(algorithme));
	   	fprintf(stderr,"sharoundsflag = %d\n",sharoundsflag);
	   	fprintf(stderr,"sharounds_str = %s\n",null2str(sharounds_str));
	   	fprintf(stderr,"sharounds = %d\n",sharounds);
	   	fprintf(stderr,"verboseflag = %d\n",verboseflag);

	   }

	
	/*
 	 * invalid arguments handling
	 */

#ifndef _WITHPAM

	if(pamflag) {

		fprintf(stderr,"%s: Sorry, this version is compiled without pam support.\n\n",prgname);
	   	hlpflag=1;

	}

#endif
	   if (pamflag && nopamflag) {

		   fprintf(stderr,"%s: Can't enable pam and nopam.\n\n",prgname);
		   hlpflag=1;

	   }
	   else {

		   if ( pamflag && ( md5flag || encryptflag || algoflag ) ) {

			   fprintf(stderr,"%s: --md5, --crypt-method and --encrypted are invalid with forced pam.\n\n",prgname);

			   hlpflag=1;

		   }
		   else {
		   	if ( md5flag || encryptflag || algoflag ) {

				nopamflag=1;

				if (md5flag && encryptflag) {

					fprintf(stderr,"%s: Can't use --md5 and --encrypted at the same time\n\n",prgname);
					hlpflag=1;

				}

				if (md5flag && algoflag) {

					fprintf(stderr,"%s: Can't use --md5 and --crypt-method at the same time\n\n",prgname);
					hlpflag=1;

				}

				if (algoflag && encryptflag) {

					fprintf(stderr,"%s: Can't use --crypt-method and --encrypted at the same time\n\n",prgname);
					hlpflag=1;

				}

			} else {

				if (sharoundsflag) {

					if (!algoflag  ) {

						fprintf(stderr,"%s: Sorry --sha-rounds is only allowed with a sha2 algorithm\n\n",prgname);
						hlpflag=1;

					} else {

						switch (algomode) {

							case 3:
							case 4:
								break;
							default:
								fprintf(stderr,"%s: 2 Sorry --sha-rounds is only allowed with a sha2 algorithm\n\n",prgname);
								hlpflag=1;
								break;


						}
					}

				}

			}

		   }

	   }

	   /*
 	    *  display usage and exit if hlpflag is enabled.
 	    */

	   if(hlpflag) {

	   	usage();
	   	exit(1);

	   }

   }
   

#ifdef _WITHPAM

  /*
   * enable pam by default on pam systems
   */

  if (!nopamflag) {

	   pamflag=1;

  }

#ifdef NO_CHPASS_CRYPT

  /* 
   * on pam only system nopam is not available
   */

  else {

	  fprintf(stderr,"%s: is compiled without --nopam support\n\n",prgname);
	  exit(1);

  }

#endif
   
   set_pam_service("passwd");      /* set PAM service name */
   set_pam_chauth_flag(0);

#else

        /*
         * On non-pam systems the --pam argument is invalid
         */

	if (pamflag) {

		fprintf(stderr,"%s: is compiled without --pam support\n\n",prgname);
		exit(1);


	}
	else {

		pamflag=0;
		nopamflag=1;

	}
   
#endif

   	if ( verboseflag ) {

		fprintf(stderr,"DEBUG: option flags after evaluation\n\n");

   		fprintf(stderr,"hlpflag = %d\n",hlpflag);
   		fprintf(stderr,"encryptflag = %d\n",encryptflag);
   		fprintf(stderr,"pamflag = %d\n",pamflag);
   		fprintf(stderr,"nopamflag = %d\n",nopamflag);
   		fprintf(stderr,"md5flag = %d\n",md5flag);
	   	fprintf(stderr,"algoflag = %d\n",algoflag);
	   	fprintf(stderr,"algorithme = %s\n",null2str(algorithme));
	   	fprintf(stderr,"sharoundsflag = %d\n",sharoundsflag);
	   	fprintf(stderr,"sharounds_str = %s\n",null2str(sharounds_str));
	   	fprintf(stderr,"sharounds = %d\n",sharounds);
   		fprintf(stderr,"verboseflag = %d\n",verboseflag);

   	}

	switch(algomode) {

		case 3:
		case 4:
			if(sharoundsflag) {

				if (verboseflag) {

					fprintf(stderr,"DEBUG: set sha2 rounds to %d\n",sharounds);

				}

				set_sha2_prefered_rounds(sharounds);

				if(get_sha2_prefered_rounds() != sharounds ) {

					fprintf(stderr,"ERROR: set sha2rounds failed\n");
					exit(3);


				}

			}

	}


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

      			fprintf(stderr,"\n%s: Too much data...\n\n",prgname);  /* buffer too small */
      			exit(1);

   		}


		/*
		 * chop
		 */

		if (line[strlen(line)-1] == '\n' ) {

   			line[strlen(line)-1]='\0'; 

		}

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

      			fprintf(stderr,"\n%s: Input error no \":\" in input on line: %d\n\n",prgname,linecounter+1);
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
   
   		pass=(char *) xmalloc(strlen(c)+1);
   		/* strncpy(pass,c+1,strlen(c)-1); */
   		strcpy(pass,c+1);
   		/* pass[strlen(c)]='\0'; */
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

/*
		if (verboseflag) fprintf(stderr,"DEBUG \"%s\" \"%s\"\n",names[linecounter],passwords[linecounter]);

*/

		name=names[linecounter];
		pass=passwords[linecounter];

		pw=get_pw_info(name,pamflag);


   		if (!pw) {

      			fprintf(stderr,"\n%s: get_pw() failed. Failed to get password info for user: %s on line: %d\n",prgname,name,linecounter+1); /* get_pw() failed */
			fprintf(stderr,"%s: Errors detected, changes ignored\n\n",prgname);
      			exit(1);

   		};

	}

	/*
	 * walk again to update the passwords
	 */

	for (linecounter=0;linecounter<number_of_lines;++linecounter) {

/*
		if (verboseflag) fprintf(stderr,"DEBUG \"%s\" \"%s\"\n",names[linecounter],passwords[linecounter]);
*/


		name=names[linecounter];
		pass=passwords[linecounter];

		pw=get_pw_info(name,pamflag);


   		if (!pw) {

      			fprintf(stderr,"\n%s: Failed to get password info for user: %s on line: %d\n",prgname,name,linecounter+1); /* get_pw() failed */
			error=1;
      			continue;

   		};

#ifdef _WITHPAM

   		if (pamflag) {

			if (verboseflag) fprintf(stderr,"DEBUG: running chpw() for user \"%s\"\n",name);
			i=chpw(pw,pass);

   		}
   		else {

#endif

	   		int mode=255;

			if (algoflag) {

				mode=algomode;

			} else {

	   			if (md5flag) {

		   			mode=1;

	   			}
	   			else {

		   			if(encryptflag) {

			   			mode=254;

		   			}
	   			}

			}

			if (verboseflag) {

				 fprintf(stderr,"DEBUG: running chpw_nopam() for user %s\n",name);
				 fprintf(stderr,"DEBUG: mode = %d\n",mode);

			}

	   		i=chpw_nopam(pw,pass,mode);

			if (i==-3) {

				fprintf(stderr,"\n%s: failed to create lock %s",prgname,TMPLOCK);


			}

			if (verboseflag) {
				fprintf(stderr,"DEBUG: chpw_nopam() return code = %d \n",i);
				fprintf(stderr,"DEBUG: str_passerror = \"%s\"\n",str_passerror(i));
			}

#ifdef _WITHPAM
   		}
#endif

/*
		if (verboseflag) fprintf(stderr,"DEBUG: running chpw() (_WITHPAM is disabled) for user %s\n",name);
   		i=chpw(pw,pass);
*/


   		if (i!=PASS_SUCCESS) {

      			/* chpw() failed */
      			fprintf(stderr,"\n%s: Can't update password for user %s on line %d",prgname,name,linecounter+1);
#ifdef _WITHPAM

      			if (pamflag) {

      				fprintf(stderr,"\n%s: pam return code = %d\n",pam_strerror(pw->pamh,i),i);
				fprintf(stderr,"\n errno = %d %s\n",errno,strerror(errno));

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

		fprintf(stderr,"\n%s: Sorry, couldn't update all passwords\n",prgname);

	}
   
   	fprintf(stderr,"%s: %d of %d passwords updated...\n\n",prgname,pass_updated,number_of_lines);
   	exit(error);

}
