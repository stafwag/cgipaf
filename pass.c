/*
 * pass.c
 * 
 * (GPL) 2000,2002 Belgium                           http://staf.patat.org
 * Staf Wagemakers                                          staf@patat.org
*/
#include "pass.h"

/*
 * if BSDHOST is not defined assume /etc/passwd
 */
#ifndef BSDHOST
#define PASSWDFILE	"/etc/passwd"
#else
#define PASSWDFILE	_PATH_MASTERPASSWD
#endif

#ifdef MD5_CRYPT
#include "md5crypt.h"
#endif
#include "salt.h"

static char *passwd_location=NULL;
static char *shadow_location=NULL;

/*
 * Set the password file location 
 * NULL = return current location
 */
char * set_passwd_location ( char * passwd_file ) {
	if(passwd_file!=NULL) passwd_location=passwd_file;
	return(passwd_location);
}

/*
 * Set to passwd_location to NULL
 * ( use system passwd )
 */
void reset_passwd_location () {
	passwd_location=NULL;
}

/*
 * Set the shadow file location
 * NULL = return the current location
 */
char * set_shadow_location ( char * shadow_file ) {
	if(shadow_file!=NULL) shadow_location=shadow_file;
	return(shadow_location);
}

/*
 * Set to shadow_location to NULL
 * ( use system shadow file )
 */
char * reset_shadow_location() {
	shadow_location=NULL;
}


#ifndef HAVE_SHADOW_H
/*
 * no lckpwdf so create our own
 */
int lckpwdf () 
{
struct stat st;
int lock;
int i=0;
char *passwdfile=PASSWDFILE;
for(;;) {
    if((lock = open(passwdfile,O_RDONLY,0))<0) return(-2);
    if(fcntl(lock,F_SETFD,1)==-1) return(-2);
    while (flock(lock,LOCK_EX|LOCK_NB)) {
          ++i;
          if(i>30) return(-2);
          sleep(1);
    };
    if(fstat(lock,&st) < 0) return(-2);
    if(st.st_nlink !=0 ) break;
    close(lock);
    lock=-2;
}
return(lock);   
} 
#endif

/*
 * reads the passwd info out /etc/passwd and
 * /etc/shadow
 *
 * name = loginname
 * returns         passwd info
 *                 NULL = error
*/
struct pw_info * get_pw(char *name)
{
struct pw_info *pw;
FILE *pw_file=NULL,*sh_file=NULL;
pw=(struct pw_info *) xmalloc(sizeof(struct pw_info));
pw->p=NULL;
pw->sp=NULL;

#ifndef BSDHOST
if(passwd_location==NULL) {
#endif
  	if(!(pw->p=getpwnam(name))) return(NULL);   /* User doesn't exist... */
#ifndef BSDHOST
}
else {
	if((pw_file=fopen(passwd_location,"r"))==NULL) return(NULL);
	if(!(pw->p=fgetpwnam(pw_file,name))) return(NULL);
}
#endif

  if (!strcmp(pw->p->pw_passwd,"x")) {

#ifdef HAVE_SHADOW_H

#ifndef BSDHOST
     if(set_shadow_location(NULL)==NULL) {
#endif
       if(!(pw->sp=getspnam(name))) return(NULL);
#ifndef BSDHOST
     }
     else {
	     if((sh_file=fopen(shadow_location,"r"))==NULL) return(NULL);
	     if(!(pw->sp=fgetspnam(sh_file,name))) return(NULL);
     }
#endif


#else
   /*
    * shadow.h is unavailable, unable to copy useful shadow info in pw
    */
   pw->sp=NULL;
#endif
   }
if(pw_file!=NULL) fclose(pw_file);
if(sh_file!=NULL) fclose(sh_file);
return(pw);
}

/*
 * returns 0 if p is a std password
 *         1 if p is a md5 password
 */
int is_md5password(char *p) {
   if (strncmp(p,"$1$",3) == 0) return(1);
   return(0);
}

/*
 * returns a pointer to the password field
 */
char * get_pwfield (struct pw_info *pw) 
{
     if(pw->sp==NULL) return(pw->p->pw_passwd);
#ifdef HAVE_SHADOW_H
     return(pw->sp->sp_pwdp);
#endif
     return(NULL);
};

/*
 * returns password crypt type
 *         0 = std crypt
 *         1 = md5
 */
int get_crypttype (struct pw_info *pw)
{
char *pass=get_pwfield(pw);
if (pass==NULL) return(-1);
return (is_md5password(pass));
}

/*
 * test a passwd
 * *p = passwd info
 * *pass = password
 * returns:        -1 = wrong passwd
 *                 0 = ok
*/
int ckpw(struct pw_info *pw,char *pass)
{
char *c,*p,salt[3];
if((p=get_pwfield(pw))==NULL) return(-1);  /* error! */
if (is_md5password(p)) {
   c=p;  	  /* p is a md5 password use complete password as salt */ 
   }
else {
   salt[0]=p[0];  /* p in no md5 password use std salt; */
   salt[1]=p[1];
   salt[2]='\0';
   c=salt;
}
if (strcmp(p,crypt(pass,c))) return(-1);   /* wrong password */
return(PASS_SUCCESS);
}

/*
 * update the password file
 * *pwfilename = passwd filename
 * *name = username
 * *encrypt_pass = new encrypted password
 * returns  0  = ok
 *         -1  = error open pwfilename
 *         -2  = reserved (locking)
 *         -3  = reserved (locking)
 *         -4  = error open TMPFILE
 *         -5  = fileno failed
 *         -6  = fchmod failed
 *         -7  = bufferlength too small
 *         -8  = error updating tmpfile
 *         -9  = out of memory
 *         -10 = lsstat failed
 *         -11 = rename failed
 *	   -12 = user not found
 *         -13 = reserved (unsupported crypt type)
 * 	   -14 = pw_mkdb failed ( freebsd only )
 */
int update_pwfile(char *pwfilename,struct pw_info *pw,char *encrypt_pass)
{
FILE *pwfile;
FILE *tmpfile;
int fd=0;
int uf=0;                 /* user found, set if user is found */
char buffer[BUFFERLEN];
char buffer2[BUFFERLEN];
char *cp;
struct stat st;
char *name=pw->p->pw_name;

umask(0377);

if (!(pwfile=fopen(pwfilename,"r"))) return(-1); /* can't open password file */
if (!(tmpfile=fopen(TMPFILE,"w"))) return(-4);   /* can't create tmp file    */
if ((fd=fileno(tmpfile))==-1) return(-5);        /* can't convert stream to int file */
if (fchmod(fd,256)==-1) return(-6);              /* can't chmod tmpfile */

/*
 * password file parser
 */
   
while (fgets(buffer,BUFFERLEN-1,pwfile)) { 
   if(!(strrchr(buffer,'\n'))) {errno=E2BIG;return(-7);} /* buffer overflow */
   if(!uf) {                     
   if(!(*buffer=='#' || *buffer=='\n')) {     /* if buffer is no comment or empty */
   cp=(char *)strchr(buffer,':');  /* cp point to the end of the current username */
   if(cp==NULL) return (-2);      /* ":" not found, doesn't look like a password file... */
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
#ifndef BSDHOST
   if (rename(TMPFILE,pwfilename)==-1) return(-11); 
#else

/*
 * Free|Net Bsd password changer
 * use pw_mkdb (8) to update the passwd db, TMPFILE is the input file
 */
   int pstat;
   pid_t pid;

#ifdef NETBSDHOST
/* use / if NETBSD */
   char *mppath = "/";
#else
   /* assume FREEBSD */
   char *mppath = _PATH_PWD;
#endif
   
   if(!( pid=fork() )) {
      if (!name)
         execl(_PATH_PWD_MKDB,"pwd_mkdb","-p","-d",mppath,TMPFILE,NULL);
      else
         execl(_PATH_PWD_MKDB,"pwd_mkdb","-p","-d",mppath,"-u",name,TMPFILE,NULL);
   }
   pid = waitpid(pid,&pstat,0);

   if (!(pid=-1 || !WIFEXITED(pstat) || WEXITSTATUS(pstat) != 0)) return(-14);
#endif
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
int i;
int fd,fd_tmplock,lock,count;
char *c;
char *passwdfile;
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

switch(get_crypttype(pw))
{
	case 0:      c=std_seed();           /* standard crypt password */
                     encrypt_pass=crypt(pass,c);
		     break;
#ifdef MD5_CRYPT
        case 1:
         	     c=md5_seed();           /* md5 password            */
#ifndef BSDHOST
		     encrypt_pass=libshadow_md5_crypt(pass,c); 
#else
		     encrypt_pass=xmalloc(_PASSWORD_LEN+1);
		     encrypt_pass[0]='\0';
		     (void)crypt_set_format("md5");
		     encrypt_pass=crypt(pass,c);
#endif
		     break;
#endif
        default:     c=NULL;                 /* unsupported crypt type! */
		     i=-13;
}

if (c!=NULL) {


/*
 * set passwdfile to the real password file
 */

if (pw->sp) { 
	if((passwdfile=shadow_location)==NULL)
		passwdfile=SHADOWFILE;
   }
   else {
	   if((passwdfile=passwd_location)==NULL) 
		   passwdfile=PASSWDFILE;
   }

/*
 * try to update the user's password
 */
i=update_pwfile(passwdfile,pw,encrypt_pass);

}

unlink(TMPLOCK);

#ifndef BSDHOST
ulckpwdf();
#endif

if (i<0) return(i);   /* can't update password */

/*
 * Solaris compatibility
 */

#ifndef BSDHOST

if (!strcmp(passwdfile,SHADOWFILE)) {
   if(lstat(OSHADOWFILE,&st)!=-1) {
      if((i=update_pwfile(OSHADOWFILE,pw,encrypt_pass))<0) return(i);
      }
   }
#endif

return(PASS_SUCCESS);
}

