/* ------------------------------------------------------------------ */
/* mail control web interface                                         */
/* author : staf wagemakers                         (c) 1999,2001-02  */
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

void yes_no(char *test,char *name,char *not_name,int *state) 
{
   if(test==NULL) return;

   if (!strcasecmp(test,txt_yes)) {
      strcpy(name,txt_yes);
      strcpy(not_name,txt_no);
      *state=1;
   }
   else {
      strcpy(name,txt_no);
      strcpy(not_name,txt_yes);
      *state=0;
   }
}

main()
{
   int    forward_state=0;
   int    keep_state=0;
   int    autoreply_state=0;
   int    oldstate=0;
   int    newstate=0;
   char   *c=NULL;
   char   *sendmail=NULL;
   char   *domain=NULL;
   char   txt_sendmail[]="/usr/lib/sendmail";
   FILE   *fp;
   WEBDATA *cookies;
#include "cgipaf_vars.h"
   fflush(0);
   unsetenv("IFS");
   
   set_memerr(out_of_memory);      /* set out of memory handler */

   /* reset parms */
   memset(forward,'\0',sizeof(forward)); 
   forward_to=txt_NULL;                  
   memset(keep_msg,'\0',sizeof(keep_msg));
   memset(autoreply,'\0',sizeof(autoreply));
     

   /* Where are my documents??? */

   if ((config_file=fopen(CONFIGFILE,"r"))!=NULL) {
      doc_root=get_sg_item(config_file,CFGSECTION,DOC_ROOT);
      
      /* is logging enabled? */
      
      if((cp=get_sg_item(config_file,CFGSECTION,CFG_SYSLOG))!=NULL) {
	 if (!strcasecmp(cp,"off")) enable_log(0);
	 free(cp);
      }
      
      /* set loglevel */
      
      if ((cp=get_config_item(config_file,CFG_LOGLEVEL))!=NULL) {
	 sscanf(cp,"%d",&brol);
	 set_loglevel(brol);
	 free(cp);
	 write_log(LOG_USER,7,"Set loglevel to %d",set_loglevel(-1));
      }
   }
   else {
      
      /* couln't open the cfgfile, print errormessage and exit */
       
      print_txt_msg(err_mcfg_configfile);
      puts(CONFIGFILE);
      puts(txt_contact_webmaster);
      write_log(LOG_USER,3,"%s %s",err_mcfg_configfile,CONFIGFILE);
      exit(0);
   }
#ifdef _WITHPAM
   
   /* set the pam service name */

   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_PAM_SERVICE))!=NULL) {
      set_pam_service(cp);
      free(cp);
      write_log(LOG_USER,7,"pam service name set to %s",set_pam_service(NULL));
   }

#endif /* _WITHPAM */

   /* set the accessdb path, if accessdb isn't configured show error and exit */
   
   if ((accessdb=get_sg_item(config_file,CFGSECTION,CFG_ACCESSDB))==NULL) {
      write_log(LOG_USER,3,"%s",err_mcfg_accessdb);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESSDB,err_mcfg_accessdb,options);
   }

   /* set the sendmail cmd path */
   
   if ((sendmail=get_section_config_item(config_file,CFGSECTION,SENDMAIL))==NULL)
     sendmail=txt_sendmail;
   write_log(LOG_USER,7,"sendmail set to %s",sendmail);
   
   /* set the mailerdomain */ 
   
   domain=get_section_config_item(config_file,CFGSECTION,CFGDOMAIN);
   if(domain!=NULL) write_log(LOG_USER,7,"domain set to %s",domain);
   else write_log(LOG_USER,7,"domain not set");

   /* set cookie timeout */
   
   c=get_section_config_item(config_file,CFGSECTION,CFG_COOKIETIMEOUT);
   if(c!=NULL) sscanf(c,"%d",&cookie_timeout);
   write_log(LOG_USER,7,"cookie_timeout set to %d",cookie_timeout);
   snprintf(cookie_timeout_txt,80,"%d",cookie_timeout);

   if (doc_root!=NULL) {
   write_log(LOG_USER,7,"doc_root set to %s",doc_root);
   if (chdir(doc_root)==-1) write_log(LOG_USER,7,"chdir(%s) failed, %s",doc_root,strerror(errno));
   }

   
   
   /* read the POST data */
   
   write_log(LOG_USER,7,"Reading POST data...");
   data=read_post();

   /* no post? show login and exit */
   
   if (!data) {
      write_log(LOG_USER,7,"no POST data, display login_document");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,LOGIN_DOCUMENT,err_readdata,options);
   }
   
   /* set name to the loginname send by viewmailcfg.cgi */
   
   name=NULL;
   if ((cp=get_postitem(data,LOGIN))!=NULL) {
      name=textarea2asc(cp);
      free(cp);
   }
   
   /* set forward and not_forward */
   
   c=get_postitem(data,FORWARD);
   yes_no(c,forward,not_forward,&forward_state);
   if(c!=NULL) free(c);

   /* set forward_to */
   
   c=get_postitem(data,FORWARD_TO);
   if(c!=NULL) {
      char *c2;
      c2=textarea2asc(c);
      if(c2==NULL) {
	 print_txt_msg("out of memory");
	 write_log(LOG_USER,0,"out of memory");
	 exit(0);
      }
      free(c);
      forward_to=xmalloc(strlen(c2)+1);
      strcpy(forward_to,c2);
      free(c2);
   }

   /* set keep_msg */
   
   c=get_postitem(data,KEEP_MSG);
   yes_no(c,keep_msg,not_keep_msg,&keep_state);
   if(c!=NULL) free(c);
   
   /* set autoreply */
   
   c=get_postitem(data,AUTOREPLY);
   yes_no(c,autoreply,not_autoreply,&autoreply_state);
   if(c!=NULL) free(c);

   /* set autoreply_msg */
   
   c=get_postitem(data,AUTOREPLY_MSG);
   if(c!=NULL) {
      autoreply_msg=textarea2asc(c);
      free(c);
   }
   
   /* copy the data in the options array */
   
   options[0][1]=name;
   options[6][1]=forward_to;
   options[13][1]=autoreply_msg;
   
   /* if name is NULL or has no lenght show error and exit */
   
   if (name==NULL) {
      write_log(LOG_AUTHPRIV,3,err_mcfg_readname);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options);
   }
   if (strlen(name)<1) {
      write_log(LOG_AUTHPRIV,3,err_mcfg_readname);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options);
   }

   /* if name="root" show error and exit */
   
   if (!strcmp(name,ROOT)) {
      write_log(LOG_AUTHPRIV,5,warn_root);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }

   /* if forward has no length show error and exit */
   
   if (strlen(forward)<1) {
      write_log(LOG_USER,5,err_forward);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_FORWARD,err_forward,options);
   }

   /* if mailforwarding is enabled evaluate forward_to and keep_msg */
   
   if (!strcasecmp(forward,"yes")) {
      write_log(LOG_USER,7,"User %s want to enable mailforwarding",name);
      
      /* forward_to has no length or is an invalid mailaddress, exit */
      
      if (strlen(forward_to)<1) {
	 write_log(LOG_USER,7,"forward_to has zero length, display err_forwardto");
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_FORWARDTO,err_forwardto,options);
      }
      if (!tst_emailaddress(forward_to)) {
	 write_log(LOG_USER,7,"forward_to has zero length, display err_forwardto");
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALIDFORWARDTO,err_invalidforwardto,options);
      } 

      /* keep_msg has no length exit */
      
      if (strlen(keep_msg)<1) {
	 write_log(LOG_USER,5,err_keep_msg);
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_KEEP_MSG,err_keep_msg,options);
      }
   }

   /* if autoreply has no length show error and exit */
   
   if (strlen(autoreply)<1) {
      write_log(LOG_USER,5,err_autoreply);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_AUTOREPLY,err_autoreply,options);
   }
   
   /* if autoreply is enabled check autoreply_msg */
   
   if ((strlen(autoreply_msg)<1)&&!strcasecmp(autoreply,txt_yes)) {
      write_log(LOG_USER,7,"autoreply is enabled, and autoreply_msg has zero length, display err_autoreplymsg");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_AUTOREPLYMSG,err_autoreplymsg,options);
   }

   /* get the user info, if the user doesn't exsist show error and exit */
   
   if (!(pw=get_pw(name))) {
      write_log(LOG_AUTHPRIV,1,"try to use mailcfg.cgi as %s which doesn't exist");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   };
   
   options[19][1]=pw->p->pw_dir;
   
   /* get the cookies */
   
   cookies=get_cookies();

   /* i need cookies or i leave */
   
   if(cookies==NULL) {
      write_log(LOG_AUTHPRIV,1,"get_cookies() failed");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIE,err_cookie,options);
   }
   else {
      
      /* i want my cookie or i quit */
      
      if((cookie=get_cookie(cookies,"cgipaf"))==NULL) {
	 write_log(LOG_AUTHPRIV,1,"get_cookie() failed");
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
      }
      else{
	 
	 /* i dont like old cookies */
	 
	 if (!cmp_access_cookie(accessdb,name,cookie,cookie_timeout)) {
	    write_log(LOG_AUTHPRIV,6,"cookie timeout for user %s",name);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIETIMEOUT,err_cookietimeout,options);
	 }
      }
   }

   /* login=ok, god becomes human */
   
   write_log(LOG_USER,7,"set gid to %d",pw->p->pw_gid);
   if (setgid(pw->p->pw_gid)==-1) {
      write_log(LOG_USER,7,"setgid() failed");
   }
   write_log(LOG_USER,7,"set uid to %d",pw->p->pw_uid);
   if (setuid(pw->p->pw_uid)==-1) {
      write_log(LOG_USER,7,"setuid failed");
   }
   write_log(LOG_USER,7,"set umask to 0177");
   umask(0177);

   /* get the previos mailcfg state out the user's .cgipaf_state file */

   if((oldstate=get_mailcfg_status(pw))==-1) 
     write_log,(LOG_USER,7,"failed to read old status file .cgipaf_state: %s for user %s",strerror(errno));
   
   /* calculate the new state */
   
   newstate=forward_state+2*keep_state+4*autoreply_state; 
   
   if(oldstate==-1) oldstate=0;  /* if .cgipaf_state doesn; exists asume state 0 */
   
   /* if not configured  -> configured
    * run run_before_mailcfg
    */
   
   if(!oldstate&&newstate) {
      i=run_cmd(config_file,CFGSECTION,RUN_BEFORE_MAILCFG,options);
      if(i<0) {
	 if(i==-1)
	   write_log(LOG_USER,1,"Can't executed %s %s",RUN_BEFORE_MAILCFG,strerror(errno));
      }
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_USER,1,"run_before returns a non-null exitcode %d",i);
   }
   
   /* if configured -> not configured
    * run run_after_mailcfg
    */
   
   if(oldstate&&!newstate) {
      i=run_cmd(config_file,CFGSECTION,RUN_AFTER_MAILCFG,options);
      if(i<0) {
	 if(i==-1)
	   write_log(LOG_USER,1,"Can't executed %s %s",RUN_BEFORE_MAILCFG,strerror(errno));
      }
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_USER,1,"run_after_mailcfg returns a non-null value %d",i);
   }

   if((cp=get_sg_item(config_file,CFGSECTION,RUN_MAILCFG))!=NULL) {
      free(cp);
      i=run_cmd(config_file,CFGSECTION,RUN_MAILCFG,options);
      if(i<0)
	write_log(LOG_USER,1,"run_mailcfg failed, run_cmd() returns %d",i);
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_USER,1,"run_mailcfg returns a non-null value %d",i);
      if(i!=0) {
	 snprintf(mailcfg_exitcode_txt,80,"%d",i);
	 if(show_msg(config_file,doc_root,CFGSECTION,ERR_MAILCFGSCRIPT,err_mailcfgscript,options)==2) printf("%d",i);
	 if(config_file!=NULL) fclose(config_file);
	 exit(0);
      }
   }
   else {
      /* deletes the user's .forward */
      
      c=add2home(pw->p,DOTFORWARD);
      if (c!=NULL) {
	 if (unlink(c)==-1) {
	    if (!(errno==ENOENT)) {
	       free(c);
	       write_log(LOG_USER,1,err_delforward);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_DELFORWARD,err_delforward,options);
	    } 
	 }
	 free(c);
      }
 
      /* deletes the user's .procmail */
   
      c=add2home(pw->p,PROCMAIL);
      if (c!=NULL) {
	 if (unlink(c)==-1) {
	    if (!(errno==ENOENT)) {
	       free(c);
	       write_log(LOG_USER,1,err_delprocmail);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_DELPROCMAIL,err_delprocmail,options);
	    } 
	 }
	 free(c);
      }
      
      /* copy the user's messages to $HOME/vacations.txt */
      
      if(!(fp=fopen(add2home(pw->p,"vacations.txt"),"w"))) {
	 write_log(LOG_USER,1,err_openvacations);	
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_OPENVACATIONS,err_openvacations,options);
      }
      fputs(autoreply_msg,fp);
      fclose(fp);
      
      /* if auto replay if enabled create a new .procmailrc */
      
      if(!strcmp(autoreply,"yes")||!strcmp(forward,"yes")) {
	 if(write_procmailrchead(pw,sendmail)==-1) {
	    write_log(LOG_USER,1,"%s write_procmailrchead() failed",err_updateprocmailrc);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options);
	 }
      }
      
      if (!strcmp(autoreply,"yes")) {
	 if(enable_reply(pw,domain)==-1) {
	    write_log(LOG_USER,1,"%s enable_reply() failed",err_updateprocmailrc);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options);
	 }
      }
      
      /* enable mail forwardin in .procmailrc */
      
      if (!strcmp(forward,"yes")) {
	 if (!strcmp(keep_msg,"yes")) {
	    if (enable_kforward(pw,textarea2asc(forward_to),domain)==-1) {
	       write_log(LOG_USER,1,"%s enable_kforward() failed",err_updateprocmailrc);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options);
	    }
	 }
	 else {
	    if(enable_forward(pw,textarea2asc(forward_to),domain)==-1) {
	       write_log(LOG_USER,1,"%s enable_forward() failed",err_updateprocmailrc);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options);
	    }
	 }
      }
   }
   
   /* mail config update, inform the user */
      
   show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_updated,options);
   write_log(LOG_AUTHPRIV,6,"User %s has updated his mail configuration successfully",name);
      
   /* create the user's .cgipaf_state */
      
   if(save_mailcfg_status(pw->p,forward_state,textarea2asc(forward_to),keep_state,autoreply_state)==-1) {
      write_log(LOG_USER,1,"Can't create statefile %s",strerror(errno));
   }
   
   /* start run_success if defined */
   
   i=run_cmd(config_file,CFGSECTION,RUN_SUCCESS,options);
   if(i==-1)
     write_log(LOG_USER,1,"Can't executed run_success %s",strerror(errno));
   else
     if((i=WEXITSTATUS(i)))
       write_log(LOG_USER,1,"run_succes returns a non-null value",i);
   if(config_file!=NULL) fclose(config_file);
}
