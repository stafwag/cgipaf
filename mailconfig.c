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
int get_reply(struct pw_info *pw)
{
FILE *fp;
char buf[100];
char *c;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"r"))) return(0);
c=fgets(buf,100,fp);
fclose(fp);
if (!c) return(0);
if (buf[0]!='S') return(0);
return(1);
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
int get_forward(struct pw_info *pw,char *mailadres)
{
FILE *fp;
char *c,buf[100];
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"r"))) return(0);
while (fgets(buf,100,fp)) {
    c=strstr(buf,":0");
    if (c!=NULL) if (strlen(c)>3) c=NULL;
    if (c!=NULL) break; 
}

if (c==NULL) {fclose(fp); return(0);}
if (!(fgets(buf,100,fp))) {fclose(fp);return(0);}
strcpy(mailadres,buf+2);
fclose(fp);
return(1);
}

int get_kforward(struct pw_info *pw,char *mailadres)
{
FILE *fp;
char *c,buf[100];
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"r"))) return(0);
while (fgets(buf,100,fp)) {
    c=strstr(buf,":0 c");
    if (c!=NULL) break;
}
if (c==NULL) {fclose(fp); return(0);}
if (!(fgets(buf,100,fp))) {fclose(fp);return(0);}
strcpy(mailadres,buf+2);
fclose(fp);
return(1);
}
int enable_reply(struct pw_info *pw,char *sendmail,char *domain)
{
FILE *fp;
char domainname[100],hostname[100];
char hostdomainname[200];
char *loopdomain;
if (domain!=NULL) loopdomain=domain;
  else {
       hostdomainname[0]='\0';
       gethostname(hostname,99);
       searchdomain(domainname,99);
       if(hostname!=NULL) strcat(hostdomainname,hostname);
       strcat(hostdomainname,".");
       if(domainname!=NULL) strcat(hostdomainname,domainname);
       loopdomain=hostdomainname;
  }
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"w"))) return(0);
fputs("SENDMAIL=",fp);fputs(sendmail,fp);fputs("\n",fp);
fputs(":0 h c\n",fp);
fputs("* !^FROM_DAEMON\n",fp);
fprintf(fp,"* !^X-Loop: %s@%s\n",pw->p->pw_name,loopdomain);
fputs("| (formail -r -A\"Precedence: junk\" \\\n",fp);
fprintf(fp,"-A\"X-Loop: %s@%s\" ; \\\n",pw->p->pw_name,loopdomain);
fputs("cat $HOME/vacations.txt) | $SENDMAIL -t\n",fp);
fclose(fp);
return(1);
}
int enable_forward(struct pw_info *pw,char *mailadres)
{
FILE *fp;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(0);
fputs(":0\n",fp);
fprintf(fp,"! %s",mailadres);
fclose(fp);
}
int enable_kforward(struct pw_info *pw, char *mailadres)
{
FILE *fp;
if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(0);
fputs(":0 c\n",fp);
fprintf(fp,"! %s",mailadres);
fclose(fp);
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

