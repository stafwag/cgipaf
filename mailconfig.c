/*
 * mailconfig.c                                        (c) Staf Wagemakers
 */

#include "mailconfig.h"
#define  PROCMAIL ".procmailrc"
/* 
 * return $HOME/dir
 */
char * add2home(struct passwd *p,char *dir)
{
char *c;
c=(char *)xmalloc(strlen(p->pw_dir)+strlen("/")+strlen(dir)+1);
strcpy(c,p->pw_dir);
strcat(c,"/"); 
strcat(c,dir);
return(c);
}
int write_procmailrchead(struct pw_info *pw, char *sendmail)
{
FILE *fp;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"w"))) return(-1);
fputs("SENDMAIL=",fp);fputs(sendmail,fp);fputs("\n",fp);
fputs("SHELL=/bin/sh\n",fp);
fclose(fp);
return(1);
}

int get_reply(struct pw_info *pw)
{
FILE *fp;
char buf[100];
char *c;
int  ret=0;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"r"))) return(ret);
do {
   c=fgets(buf,100,fp);
   if (strstr(buf,"vacations.txt")) {
      ret=1;
      break;
   }
} while(c!=NULL);   
fclose(fp);
return(ret);
}
char * get_vacations(struct pw_info *pw)
{
FILE *fp;
char buf[100];
char *c,*cc;
cc=(char *)xmalloc(sizeof(char));
cc[0]='\0';
if(!(fp=fopen(add2home(pw->p,"vacations.txt"),"r"))) return(NULL);
do {
   c=fgets(buf,100,fp);
   if (c!=NULL) {
      rmchar(buf,'\13');
      rmchar(buf,'\10');
      cc=(char *)xrealloc(cc,strlen(buf)+strlen(cc)+1);
      strcat(cc,buf);
      }
 } while (c!=NULL);
fclose(fp);
if(cc[strlen(cc)]=='\n') {
   cc[strlen(cc)]='\0';
   cc=xrealloc(cc,strlen(cc)+1);
}
return(cc);
}
char * realget_forward(struct pw_info *pw,int mode)
{
FILE *fp;
char *c,buf[1000];
char needle_forward[]=":0";
char needle_kforward[]=":0 c";
char *needle;
char *mailadres=xmalloc(1000);
int i;
   
if(mode) needle=needle_kforward;
   else  needle=needle_forward;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"r"))) return(NULL);
i=0;
while (fgets(buf,1000,fp)) {
    buf[strlen(buf)-1]='\0';
    if (!strcmp(buf,needle)) {
       i=1;
       break;
    }
}

if (!i) {fclose(fp); return(NULL);}
if (!(fgets(buf,1000,fp))) {fclose(fp);return(NULL);}
if (strstr(buf,"X-Loop")==NULL) {
   strcpy(mailadres,buf+2);
}
else {
   while(fgets(buf,1000,fp)) {
     if((c=strstr(buf,"$SENDMAIL -oi"))) {
	c=mv_2_next((c+strlen("$SENDMAIL -oi")));
	strcpy(mailadres,c);
	break;
     }
   }
}
fclose(fp);
mailadres=xrealloc(mailadres,strlen(mailadres)+1);
return(mailadres);
}

char * get_forward(struct pw_info *pw)
{
return(realget_forward(pw,0));
}

char * get_kforward(struct pw_info *pw)
{
return(realget_forward(pw,1));
}

char *get_maildomain(char *domain)
{
char domainname[100],hostname[100];
char *hostdomainname;
hostdomainname=xmalloc(200);
if (domain!=NULL) return(domain);
hostdomainname[0]='\0';
gethostname(hostname,99);
searchdomain(domainname,99);
if(hostname!=NULL) strcat(hostdomainname,hostname);
strcat(hostdomainname,".");
if(domainname!=NULL) strcat(hostdomainname,domainname);
return(hostdomainname);
}

int enable_reply(struct pw_info *pw,char *domain)
{
FILE *fp;
char *loopdomain=get_maildomain(domain);
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(-1);
fputs(":0 h c\n",fp);
fputs("* !^FROM_DAEMON\n",fp);
fprintf(fp,"* !^X-Loop: %s@%s\n",pw->p->pw_name,loopdomain);
fputs("| (formail -r -A\"Precedence: junk\" \\\n",fp);
fprintf(fp,"-A\"X-Loop: %s@%s\" ; \\\n",pw->p->pw_name,loopdomain);
fputs("cat $HOME/vacations.txt) | $SENDMAIL -t\n",fp);
fclose(fp);
if(loopdomain!=domain) if(loopdomain!=NULL) free(loopdomain);
   return(1);
}
int write_forwardbody(FILE *fp,struct pw_info *pw,char *mailadres,char *domain)
{
char *loopdomain=get_maildomain(domain);
fprintf(fp,"* !^X-Loop: %s@%s\n",pw->p->pw_name,loopdomain);
fprintf(fp,"| formail -A \"X-Loop: %s@%s\" | \\\n",pw->p->pw_name,loopdomain);
fprintf(fp,"$SENDMAIL -oi %s",mailadres);
return(0);
}
int enable_forward(struct pw_info *pw,char *mailadres,char *domain)
{
FILE *fp;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(-1);
fputs(":0\n",fp);
write_forwardbody(fp,pw,mailadres,domain);
fclose(fp);
return(1);
}

int enable_kforward(struct pw_info *pw, char *mailadres,char *domain)
{
FILE *fp;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(-1);
fputs(":0 c\n",fp);
write_forwardbody(fp,pw,mailadres,domain);
fclose(fp);
return(1);
}
int tst_emailaddress(char *emailaddress)
{
   char *cp,*address=NULL;
   int i,ret=0;
   address=mv_2_next(emailaddress);
   cut_space(address);
   cp=strchr(address,'@');
   if (cp!=NULL) {
      if (cp!=address) {
         ++cp;
         i=(cp-address);
         cp=strchr(address+i,'.');
         if(cp!=NULL) if((cp!=(address+i))&&(cp!=(address+strlen(address)-1))) ret=1;
      }
   }
   return(ret);
}

