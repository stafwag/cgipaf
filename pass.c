/* ---------------------------------------------------------------------- */
/* pass.c						                  */
/*									  */
/* (GPL) 2000 Belgium                        http://stafwag.home7.dk3.com */
/* Staf Wagemakers                            staf.wagemakers@advalvas.be */
/* ---------------------------------------------------------------------- */
#include "pass.h"
/* ---------------------------------------------- */
/* retruns a random seed with valid ascii char's  */
/*                                                */
/* s should be a random integer                   */
/* ---------------------------------------------- */
int gen_random(int s)
{
int i;
srand(s);
i=rand();
i=i/(RAND_MAX/63);
if(i<0) i=0;
if(i>63) i=63;
if(i<12) return (i+46);
if((i>=12)&&(i<38)) return (i+(65-12));
if(i>=38) return (i+(97-38));
}
/* ---------------------------------------------- */
/* reads the passwd info out /etc/passwd and      */
/* /etc/shadow					  */
/*                                                */
/* name = loginname				  */
/* returns         passwd info		          */
/*                 0 = error                      */
/* ---------------------------------------------- */
struct pw_info * get_pw(char *name)
{
struct pw_info *pw;
pw=(struct pw_info *) xmalloc(sizeof(struct pw_info));
pw->p=NULL;
pw->sp=NULL;
if(!(pw->p=getpwnam(name))) return(NULL);
if (!strcmp(pw->p->pw_passwd,"x")) {
#ifdef HAVE_SHADOW_H
   if(!(pw->sp=getspnam(name))) return(NULL);
#else
   /*
    * shadow.h is unavailable, unable to copy useful shadow info in pw
    */
   pw->sp=NULL;
#endif
   }
return(pw);
}
/* ---------------------------------------------- */
/* test a passwd                                  */
/* *p = passwd info				  */
/* *pass = password				  */
/* returns:        -1 = wrong passwd              */
/*                 0 = ok			  */
/* ---------------------------------------------- */
int ckpw(struct pw_info *pw,char *pass)
{
char c[3],*p;
if (pw->sp==NULL) p=pw->p->pw_passwd;
#ifdef HAVE_SHADOW_H
   else p=pw->sp->sp_pwdp;
#endif
c[0]=p[0];
c[1]=p[1];
c[2]='\0';
if (strcmp(p,crypt(pass,c))) return(-1);
return(PASS_SUCCESS);
}
/* ---------------------------------------------- */
/* update the password file                       */
/* *pwfilename = passwd filename                  */
/* *name = username				  */
/* *encrypt_pass = new encrypted password         */
/* returns  0  = ok                               */
/*         -1  = error open pwfilename            */
/*         -2  = reserved (locking)		  */
/*         -3  = reserved (locking)	          */
/*         -4  = error open TMPFILE               */
/*         -5  = fileno failed                    */
/*         -6  = fchmod failed                    */
/*         -7  = bufferlength too small           */
/*         -8  = error updating tmpfile           */
/*         -9  = out of memory                    */ 
/*         -10 = lsstat failed                    */
/*         -11 = rename failed                    */
/*	   -12 = user not found			  */
/* ---------------------------------------------- */
int update_pwfile(char *pwfilename,char *name,char *encrypt_pass)
{
FILE *pwfile;
FILE *tmpfile;
int fd=0;
int uf=0;                 /* user found, set if user is found */
char buffer[BUFFERLEN];
char buffer2[BUFFERLEN];
char *cp;
struct stat st;

umask(0377);

if (!(pwfile=fopen(pwfilename,"r"))) return(-1); /* can't open password file */
if (!(tmpfile=fopen(TMPFILE,"w"))) return(-4);   /* can't create tmp file    */
if ((fd=fileno(tmpfile))==-1) return(-5);        /* can't convert stream to int file */
if (fchmod(fd,256)==-1) return(-6);              /* can't chmod tmpfile */

/*
 * password file parser
 */
   
while (fgets(buffer,100,pwfile)) { 
   if(!(strrchr(buffer,'\n'))) {errno=E2BIG;return(-7);} /* buffer overflow */
   if(!uf) {
   cp=(char *)strchr(buffer,':');  /* cp point to the end of the current username */
   if(cp==NULL) return (-12);      /* ":" not found, doesn't look like a password file... */
   strncpy(buffer2,buffer,cp-buffer);
   buffer2[cp-buffer]='\0';        /* buffer2 = current username */
   if (strcmp(name,buffer2)==0) {
      uf=1;                        /* found the user ready to update his password */
      strcpy(buffer2,buffer);
      cp=(char *)strchr(buffer2,':');
      cp=(char *)strchr(cp+1,':'); /* cp point to the end of password field */
      if ((strlen(cp)+strlen(name)+strlen(encrypt_pass))>BUFFERLEN) {
         errno=E2BIG;
         return(-7);               /* buffer overflow */
	 }
      strcpy(buffer,name);
      
      /*
       * create new passwd line
       */
      
      strcat(buffer,":");
      strcat(buffer,encrypt_pass);
      strcat(buffer,cp);
   }
   }
   /*
    * copy the buffer to the temp file
    */
   
   if (fputs(buffer,tmpfile) == EOF) return(-8);
} 
fclose(tmpfile);
fclose(pwfile);
/*
 * update to permissions of tmpfile to the permissions of pwfilename
 */
if (lstat(pwfilename,&st)==-1) return(-10);
chmod(TMPFILE,st.st_mode);
chown(TMPFILE,st.st_uid,st.st_gid);

if (uf) {
   /* user found, rename tmpfile to pwfile */
   if (rename(TMPFILE,pwfilename)==-1) return(-11); 
   }
   else { 
      /* user not found, nothing to do */
      errno=0; 
      return(-12);
   }
errno=0;
return(PASS_SUCCESS);
}
/*
 * update the user's password
 * 
 * returns:
 *             -2:  lockfile exists
 *             -3:  can't create lockfile
 */

int chpw(struct pw_info *pw,char *pass)
{
FILE *pwfile;
FILE *tmpfile;
int fd,fd_tmplock,lock,count,i;
char c[3];
char passwdfile[50];
char *encrypt_pass;
struct stat st;

/*
 * wait until the lockfile is unlocked
 */
count=10;
while (count--) {
   if ((lock=lckpwdf())!=-2) break;
   sleep(3);
   } 

/*
 * lockfile exists and we've waited long enough
 */
   
if (lock==-1) return(-2);

/* 
 * create lockfile
 */
   
count=10;
while (count--) {
   if ((fd_tmplock=open(TMPLOCK, O_RDWR | O_CREAT | O_EXCL, 0444))==-1) {
      sleep(3);
      }
      else { 
         close(fd_tmplock);
	 break;
	 }
      }

/*
 * can't create lockfile
 */
   
if (fd_tmplock==-1) return(-3); /* can't create lock */

/*
 * create a random seed
 */
   
srand(getpid());
c[0] = gen_random(time(0)-rand());
c[1] = gen_random(time(0)+getpid());
c[2] = '\0';

/*
 * encrypt the password
 */
   
encrypt_pass=xmalloc(strlen(pass)+1);
encrypt_pass=crypt(pass,c);

/*
 * set passwdfile to the real password file
 */

if (pw->sp) strcpy(passwdfile,SHADOWFILE);
  else strcpy(passwdfile,"/etc/passwd");

/*
 * try to update the user's password
 */
i=update_pwfile(passwdfile,pw->p->pw_name,encrypt_pass);

unlink(TMPLOCK);
ulckpwdf();

if (i<0) return(i);   /* can't update password */

/*
 * Solaris compatibility
 */

if (!strcmp(passwdfile,SHADOWFILE)) {
   if(lstat(OSHADOWFILE,&st)!=-1) {
      if((i=update_pwfile(OSHADOWFILE,pw->p->pw_name,encrypt_pass))<0) return(i);
      }
   }

return(PASS_SUCCESS);
}
