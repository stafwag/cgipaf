/* ---------------------------------------------------------------------- */
/* pass.c						                  */
/*									  */
/* (GPL) 2000 Belgium                          http://www.stafwag.f2s.com */
/* Staf Wagemakers                                        stafwag@f2s.com */
/* ---------------------------------------------------------------------- */
#include "pass.h"
/* ---------------------------------------------- */
/* retruns a random seed with valid ascii char's  */
/*                                                */
/* s should be random integer                     */
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
   if(!(pw->sp=getspnam(name))) return(NULL);
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
   else p=pw->sp->sp_pwdp;
c[0]=p[0];
c[1]=p[1];
c[2]='\0';
if (strcmp(p,crypt(pass,c))) return(-1);
return(0);
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
int fd,uf=0;
char buffer[BUFFERLEN];
char buffer2[BUFFERLEN];
char *cp;
struct stat st;

umask(0377);
if (!(pwfile=fopen(pwfilename,"r"))) return(-1);
if (!(tmpfile=fopen(TMPFILE,"w"))) return(-4);
if ((fd=fileno(tmpfile))==-1) return(-5);
if (fchmod(fd,256)==-1) return(-6);
   
while (fgets(buffer,100,pwfile)) { 
   if(!(strrchr(buffer,'\n'))) {errno=E2BIG;return(-7);}
   if(!uf) {
   cp=(char *)strchr(buffer,':');
   if(cp==NULL) return (-12);
   strncpy(buffer2,buffer,cp-buffer);
   buffer2[cp-buffer]='\0';
   if (strcmp(name,buffer2)==0) {
      uf=1;
      strcpy(buffer2,buffer);
      cp=(char *)strchr(buffer2,':');
      cp=(char *)strchr(cp+1,':');
      if ((strlen(cp)+strlen(name)+strlen(encrypt_pass))>BUFFERLEN) {
         errno=E2BIG;
         return(-7);
	 }
      strcpy(buffer,name);
      strcat(buffer,":");
      strcat(buffer,encrypt_pass);
      strcat(buffer,cp);
   }
   }
   if (fputs(buffer,tmpfile) == EOF) return(-8);
} 
fclose(tmpfile);
fclose(pwfile);
if (lstat(pwfilename,&st)==-1) return(-10);
chmod(TMPFILE,st.st_mode);
chown(TMPFILE,st.st_uid,st.st_gid);
if (uf) { if (rename(TMPFILE,pwfilename)==-1) return(-11); }
   else { errno=0; return(-12);}
errno=0;
return(0);
}

int chpw(struct pw_info *pw,char *pass)
{
FILE *pwfile;
FILE *tmpfile;
int fd,fd_tmplock,lock,count,i;
char c[3];
char passwdfile[50];
char *encrypt_pass;
struct stat st;

count=10;
while (count--) {
   if ((lock=lckpwdf())!=-2) break;
   sleep(3);
   } 
if (lock==-1) return(-2);

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
if (fd_tmplock==-1) return(-3);


srand(getpid());
c[0] = gen_random(time(0)-rand());
c[1] =gen_random(time(0)+getpid());
c[2] = '\0';
encrypt_pass=xmalloc(strlen(pass)+1);
encrypt_pass=crypt(pass,c);
if (pw->sp) strcpy(passwdfile,SHADOWFILE);
  else strcpy(passwdfile,"/etc/passwd");
i=update_pwfile(passwdfile,pw->p->pw_name,encrypt_pass);
unlink(TMPLOCK);
ulckpwdf();
if (i<0) return(i);
if (!strcmp(passwdfile,SHADOWFILE)) {
   if(lstat(OSHADOWFILE,&st)!=-1) {
      if((i=update_pwfile(OSHADOWFILE,pw->p->pw_name,encrypt_pass))<0) return(i);
      }
   }
   

return(0);
}