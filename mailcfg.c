/* ------------------------------------------------------------------ */
/* mail control web interface                                         */
/* author : staf wagemakers                          date: 26 NOV 99  */
/* ------------------------------------------------------------------ */
#define  FORWARD               "forward"
#define  FORWARD_TO            "forward_to"
#define  AUTOREPLY             "autoreply"
#define  AUTOREPLY_MSG         "autoreply_msg"
#define  PROCMAIL              ".procmailrc"
#define  DOTFORWARD            ".forward"
#define  KEEP_MSG              "keep_msg"
#define  SENDMAIL              "sendmail"
#define  CFGDOMAIN             "domain"
#include "cgipaf_defs.h"

char txt_yes[]="yes";
char txt_no[]="no";
void yes_no(char *test,char *name,char *not_name) 
{
   if(test==NULL) return;

   if (!strcasecmp(test,txt_yes)) {
      strcpy(name,txt_yes);
      strcpy(not_name,txt_no);
   }
   else {
      strcpy(name,txt_no);
      strcpy(not_name,txt_yes);
   }
}
main()
{
char   *c=NULL;
char   *sendmail=NULL;
char   *domain=NULL;
char   txt_sendmail[]="/usr/lib/sendmail";
FILE   *fp;
WEBDATA *cookies;
#include "cgipaf_vars.h"
unsetenv("IFS");
   
set_memerr(out_of_memory);
   
memset(forward,'\0',sizeof(forward));
memset(forward_to,'\0',sizeof(forward_to));
memset(keep_msg,'\0',sizeof(keep_msg));
memset(autoreply,'\0',sizeof(autoreply));
     

/* Where are my documents??? */

if ((config_file=fopen(CONFIGFILE,"r"))!=NULL) {
   doc_root=get_sg_item(config_file,CFGSECTION,DOC_ROOT);
   if((cp=get_sg_item(config_file,CFGSECTION,CFG_SYSLOG))==NULL) {
     if (!strcasecmp(cp,"off")) enable_log(0);
     free(cp);
   }
}
else {
   print_txt_msg(err_mcfg_configfile);
   puts(CONFIGFILE);
   puts(txt_contact_webmaster);
   write_log(LOG_USER,LOG_ERR,"%s %s",err_mcfg_configfile,CONFIGFILE);
   exit(0);
   }
   
if ((accessdb=get_sg_item(config_file,CFGSECTION,CFG_ACCESSDB))==NULL) {
   write_log(LOG_USER,LOG_ERR,"%s",err_mcfg_accessdb);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESSDB,err_mcfg_accessdb,options);
}

   
if ((sendmail=get_section_config_item(config_file,CFGSECTION,SENDMAIL))==NULL)
   sendmail=txt_sendmail;
domain=get_section_config_item(config_file,CFGSECTION,CFGDOMAIN);
c=get_section_config_item(config_file,CFGSECTION,CFG_COOKIETIMEOUT);
if(c!=NULL) sscanf(c,"%d",&cookie_timeout);

snprintf(cookie_timeout_txt,80,"%d",cookie_timeout);

chdir(doc_root);

data=read_post();

if (!data) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,LOGIN_DOCUMENT,err_readdata,options);
   }

name=get_postitem(data,LOGIN);
c=get_postitem(data,FORWARD);
yes_no(c,forward,not_forward);
if(c!=NULL) free(c);
c=get_postitem(data,FORWARD_TO);
if(c!=NULL) {
   char *c2;
   c2=textarea2asc(c);
   if(c2==NULL) {
      print_txt_msg("out of memory");
      write_log(LOG_USER,LOG_EMERG,"out of memory");
      exit(0);
   }
   free(c);
   strncpy(forward_to,c2,98);
   free(c2);
}
c=get_postitem(data,KEEP_MSG);
yes_no(c,keep_msg,not_keep_msg);
if(c!=NULL) free(c);
c=get_postitem(data,AUTOREPLY);
yes_no(c,autoreply,not_autoreply);
if(c!=NULL) free(c);
c=get_postitem(data,AUTOREPLY_MSG);
if(c!=NULL) {
   autoreply_msg=textarea2asc(c);
   free(c);
}
options[0][1]=name;
options[13][1]=autoreply_msg;
if (name==NULL) {
   write_log(LOG_AUTHPRIV,LOG_ERR,err_mcfg_readname);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options);
   }

if (strlen(name)<1) {
   write_log(LOG_AUTHPRIV,LOG_ERR,err_mcfg_readname);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options);
   }

if (!strcmp(name,ROOT)) {
   write_log(LOG_AUTHPRIV,LOG_NOTICE,warn_root);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }

if (strlen(forward)<1) {
   write_log(LOG_USER,LOG_NOTICE,err_forward);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_FORWARD,err_forward,options);
   }

if (!strcasecmp(forward,"yes")) {

   if (strlen(forward_to)<1) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_FORWARDTO,err_forwardto,options);
   }
   if (!tst_emailaddress(forward_to)) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALIDFORWARDTO,err_invalidforwardto,options);
   } 

if (strlen(keep_msg)<1) {
   write_log(LOG_USER,LOG_NOTICE,err_keep_msg);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_KEEP_MSG,err_keep_msg,options);
   }
}

if (strlen(autoreply)<1) {
   write_log(LOG_USER,LOG_NOTICE,err_autoreply);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_AUTOREPLY,err_autoreply,options);
   }

if ((strlen(autoreply_msg)<1)&&!strcasecmp(autoreply,txt_yes)) {
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_AUTOREPLYMSG,err_autoreplymsg,options);
      }

if (!(pw=get_pw(name))) {
   write_log(LOG_AUTHPRIV,LOG_ALERT,"try to use mailcfg.cgi as %s which doesn't exist");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   };

cookies=get_cookies();

if(cookies==NULL) {
   write_log(LOG_AUTHPRIV,LOG_ALERT,"create_cookie() failed");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIE,err_cookie,options);
}
else {
   if((cookie=get_cookie(cookies,"cgipaf"))==NULL) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }
   else{
      if (!cmp_access_cookie(accessdb,name,cookie,cookie_timeout)) {
	 write_log(LOG_AUTHPRIV,LOG_INFO,"cookie timeout for user %s",name);
         show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIETIMEOUT,err_cookietimeout,options);
      }
   }
}

setuid(pw->p->pw_uid);
setgid(pw->p->pw_gid);
umask(0177);

c=add2home(pw->p,DOTFORWARD);
if (c!=NULL) {
   if (unlink(c)==-1) {
      if (!(errno==ENOENT)) {
	 free(c);
	 write_log(LOG_USER,LOG_ALERT,err_delforward);
         show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_DELFORWARD,err_delforward,options);
      } 
   }
   free(c);
}
 
c=add2home(pw->p,PROCMAIL);
if (c!=NULL) {
   if (unlink(c)==-1) {
      if (!(errno==ENOENT)) {
	 free(c);
	 write_log(LOG_USER,LOG_ALERT,err_delprocmail);
         show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_DELPROCMAIL,err_delprocmail,options);
      } 
   }
   free(c);
}

if(!(fp=fopen(add2home(pw->p,"vacations.txt"),"w"))) {
     write_log(LOG_USER,LOG_ALERT,err_openvacations);	
     show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_OPENVACATIONS,err_openvacations,options);
     }
fputs(autoreply_msg,fp);
fclose(fp);

if (!strcmp(autoreply,"yes")) enable_reply(pw,sendmail,domain); 
   
if (!strcmp(forward,"yes")) {
   if (!strcmp(keep_msg,"yes")) enable_kforward(pw,textarea2asc(forward_to));
      else enable_forward(pw,forward_to);
   }

show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_updated,options);
write_log(LOG_AUTHPRIV,LOG_INFO,"User %s has updated his mail configuration successfully",name);
i=run_cmd(config_file,CFGSECTION,RUN_SUCCESS,options);
if(i==-1)
  write_log(LOG_USER,LOG_ALERT,"Can't executed run_succes %s",strerror(errno));
if(config_file!=NULL) fclose(config_file);
}
