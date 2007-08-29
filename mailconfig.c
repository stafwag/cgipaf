/*
 * mailconfig.c
 *
 * Copyright (C) 1999,2000-02 Staf Wagemakers Belgie/Belgium
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

#include                 "mailconfig.h"
#define  PROCMAIL        ".procmailrc"
#define  CGIPAFSTATEFILE ".cgipaf_state"
#define  STATEFORWARD    "forward"
#define  STATEFORWARDTO  "forwardto"
#define  STATEKEEPMSG    "keepmsg"
#define  STATEAUTOREPLY  "autoreply"

static int use_statefile=0;

int use_mailcfg_statefile(int setflag) 
{
   if (setflag != -1 ) use_statefile=setflag;
   return use_statefile;
}

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

/* 
 * write the $HOME/.procmailrc header 
 */

int write_procmailrchead(struct pw_info *pw, char *sendmail,char *formail)
{
   FILE *fp;
   if(!(fp=fopen(add2home(pw->p,PROCMAIL),"w"))) return(-1);
   fputs("SENDMAIL=",fp);fputs(sendmail,fp);fputs("\n",fp);
   fputs("FORMAIL=",fp);fputs(formail,fp);fputs("\n",fp);
   fputs("SHELL=/bin/sh\n",fp);
   fclose(fp);
   return(1);
}

/*
 * autoreply enabled?
 */

int get_reply(struct pw_info *pw)
{
   FILE *fp;
   char buf[100];
   char *c;
   int  ret=0;
   if(use_statefile) return(get_mailcfg_reply(pw));
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

/*
 * get the vacation excuse
 */

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

/*
 * wrapper to get the forward_to email addres
 */

char * realget_forward(struct pw_info *pw,int mode)
{
   FILE *fp;
   char *c,buf[1000];
   char needle_forward[]=":0";
   char needle_forward2[]=":0 E";
   char needle_kforward[]=":0 c";
   char needle_kforward2[]=":0 c E";
   char *needles_forward[]={needle_forward,needle_forward2,NULL}; 
   char *needles_kforward[]={needle_kforward,needle_kforward2,NULL}; 
   char **needles;
   char **current_needle;
   char *needle;
   char *mailadres=xmalloc(1000);
   int i;
   if(mode) needles=needles_kforward;
   else  needles=needles_forward;
   if(!(fp=fopen(add2home(pw->p,PROCMAIL),"r"))) return(NULL);
   i=0;
   while (fgets(buf,1000,fp)) {
      if(buf[strlen(buf)-1]=='\n') buf[strlen(buf)-1]='\0';

      if(i&&!strstr(buf,"FROM_DAEMON")) break;
      	else i=0;

      current_needle=needles;
      for(needle=*current_needle;needle!=NULL;needle=*(++current_needle)) {

      	if (!strcmp(buf,needle)) {
	 	i=1;
	 	break;
      		}
       	}

   }

   if ((!i)||(buf==NULL)) {fclose(fp); return(NULL);}
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

/*
 * returns the forward_to address if normal forwarding is enabled
 */

char * get_forward(struct pw_info *pw)
{
   if (use_statefile) return(get_mailcfg_forward(pw));
   return(realget_forward(pw,0));
}

/* 
 * returns the forward_to if forwarding with keep msgs is enabled
 */

char * get_kforward(struct pw_info *pw)
{
   if (use_statefile) return(get_mailcfg_kforward(pw));
   return(realget_forward(pw,1));
}

/*
 * tries to the domainname
 */

char *get_maildomain(char *domain)
{
   char domainname[100],hostname[100];
   char *hostdomainname;
   if (domain!=NULL) return(domain);
   hostdomainname=xmalloc(200);
   hostdomainname[0]='\0';
   gethostname(hostname,99);
   searchdomain(domainname,99);

   if(hostname!=NULL) strcat(hostdomainname,hostname);
   strcat(hostdomainname,".");

   if(domainname!=NULL) strcat(hostdomainname,domainname);

   replace_char(hostdomainname,'(','_');
   replace_char(hostdomainname,')','_');

   return(hostdomainname);
}

/*
 * write the autoreply part to the user's .procmailrc
 */

int enable_reply(struct pw_info *pw,char *domain)
{
   FILE *fp;
   char *loopdomain=get_maildomain(domain);
   if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(-1);
   fputs(":0 h c\n",fp);
   fputs("* !^FROM_DAEMON\n",fp);
   fputs("* !^FROM_MAILER\n",fp);
   fputs("* !^MAILER\n",fp);
   fputs("* !^UUCP\n",fp);
   fputs("* !^X-Mailing-List\n",fp);
   fputs("* !^Precedence: bulk\n",fp);
   fputs("* !^Precedence: list\n",fp);
   fputs("* !^Precedence: junk\n",fp);
   fprintf(fp,"* !^X-Loop: %s@%s\n",pw->p->pw_name,loopdomain);
   fputs("| ($FORMAIL -r -A\"Precedence: junk\" \\\n",fp);
   fprintf(fp,"-A\"X-Loop: %s@%s\" ; \\\n",pw->p->pw_name,loopdomain);
   fputs("cat $HOME/vacations.txt) | $SENDMAIL -t\n",fp);
   fclose(fp);
   if(loopdomain!=domain) if(loopdomain!=NULL) free(loopdomain);
   return(1);
}

/*
 * write the forward body part the user's .procmailrc
 */

int write_forwardbody(FILE *fp,struct pw_info *pw,char *mailadres,char *domain)
{
   char *loopdomain=get_maildomain(domain);
   fprintf(fp,"* !^X-Loop: %s@%s\n",pw->p->pw_name,loopdomain);
   fprintf(fp,"| $FORMAIL -A \"X-Loop: %s@%s\" | \\\n",pw->p->pw_name,loopdomain);
   fprintf(fp,"$SENDMAIL -oi %s\n",mailadres);
   return(0);
}

/*
 * writes the forward part to the user's .procmailrc
 */

int enable_forward(struct pw_info *pw,char *mailadres,char *domain)
{
   FILE *fp;
   if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(-1);
   fputs(":0\n",fp);
   fputs("* ^FROM_DAEMON\n",fp);
   fputs("{\n",fp);
   fputs(":0 BH\n",fp);
   write_forwardbody(fp,pw,mailadres,domain);
   fputs("}\n",fp);
   fputs(":0 E\n",fp);
   write_forwardbody(fp,pw,mailadres,domain);
   fclose(fp);
   return(1);
}

/*
 * writes the keep forward part to the user's .procmailrc
 */

int enable_kforward(struct pw_info *pw, char *mailadres,char *domain)
{
   FILE *fp;
   if(!(fp=fopen(add2home(pw->p,PROCMAIL),"a"))) return(-1);
   fputs(":0 c\n",fp);
   fputs("* ^FROM_DAEMON\n",fp);
   fputs("{\n",fp);
   fputs(":0 BH c\n",fp);
   write_forwardbody(fp,pw,mailadres,domain);
   fputs("}\n",fp);
   fputs(":0 c E\n",fp);
   write_forwardbody(fp,pw,mailadres,domain);
   fclose(fp);
   return(1);
}

/*
 * Is the emailaddres valid?
 */

int tst_emailaddress(char *emailaddress)
{
   char *cp,*address=NULL;
   char invalid_chars[]={',','|','\0'};
   int i,ret=0;
   address=mv_2_next(emailaddress);
   cut_space(address);
   for(cp=invalid_chars;*cp!='\0';cp++) {
      if(strchr(address,*cp)!=NULL) return(0);
   }
   if((cp=strchr(address,'@'))==NULL) return(0);
   if(strchr((cp+1),'@')!=NULL) return(0);
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

/*
 * save the user current mail config to $HOME/.cgipaf_status
 */

int save_mailcfg_status(struct passwd *p,int forward,char *forwardto,int keep,int autoreply)
{
   FILE *fp;
   char *filename=add2home(p,CGIPAFSTATEFILE);
   if((fp=fopen(filename,"w"))==NULL) return (-1);
   if (!forward)
     keep=0;
   fputs("# CGIPAF state file\n",fp);
   fputs("# Please don't edit!!!!!!!\n",fp);
   fprintf(fp,"%s\t\t%d\n",STATEFORWARD,forward);
   fprintf(fp,"%s\t%s\n",STATEFORWARDTO,forwardto);
   fprintf(fp,"%s\t\t%d\n",STATEKEEPMSG,keep);
   fprintf(fp,"%s\t%d\n",STATEAUTOREPLY,autoreply);
   fclose(fp);
   return(0);
}

/*
 * return mailcfg  status                                                   *
 * bit 1 forward   status                                                   *
 * bit 2 keepmsg   status                                                   *
 * bit 3 autoreply status                                                   *
*/

int get_mailcfg_status(struct pw_info *pw)
{
   FILE *fp;
   int  forward=0;
   int  keepmsg=0;
   int  autoreply=0;
   int  ret=0;
   char *buf;
   char *filename=add2home(pw->p,CGIPAFSTATEFILE);
   if((fp=fopen(filename,"r"))==NULL) return(-1);
   buf=get_config(fp,STATEFORWARD);
   if(!strcmp(buf,"1")) forward=1;
   free(buf);
   buf=get_config(fp,STATEKEEPMSG);
   if(!strcmp(buf,"1")) keepmsg=1;
   free(buf);
   buf=get_config(fp,STATEAUTOREPLY);
   if(!strcmp(buf,"1")) autoreply=1;
   free(buf);
   ret=forward+keepmsg*2+autoreply*4;
   fclose(fp);
   return(ret);
}
/*
 * reads the autoreply status in the cgipaf_state file
 */
int get_mailcfg_reply(struct pw_info *pw) 
{
   FILE *fp;
   char *buf;
   char *filename=add2home(pw->p,CGIPAFSTATEFILE);
   if((fp=fopen(filename,"r"))==NULL) return(0);  /* can't open statefile, assume not yet configured */
   buf=get_config(fp,STATEAUTOREPLY);
   fclose(fp);
   if(!strcmp(buf,"1")) return 1;
   return(0);
}

char * real_get_mailcfg_forward(struct pw_info *pw,int mode) 
{
   FILE *fp;
   char *buf;
   char *filename=add2home(pw->p,CGIPAFSTATEFILE);
   int  forward=0;
   int  keep=0;
   if((fp=fopen(filename,"r"))==NULL) return(0);  /* can't open statefile, assume not yet configured */
   buf=get_config(fp,STATEFORWARD);
   if(!strcmp(buf,"1")) forward=1;
   free(buf);
   buf=get_config(fp,STATEKEEPMSG);
   if(!strcmp(buf,"1")) keep=1;
   free(buf);
   buf=NULL;
   if(!mode) {
      if(forward&&!keep) buf=get_config(fp,STATEFORWARDTO);
   }
   else {
     if(forward&&keep)
       buf=get_config(fp,STATEFORWARDTO);
   }
   return(buf);
}
char * get_mailcfg_forward(struct pw_info *pw) 
{ 
   return(real_get_mailcfg_forward(pw,0));
}

char * get_mailcfg_kforward(struct pw_info *pw) 
{ 
   return(real_get_mailcfg_forward(pw,1));
}
