/* ----------------------------------------------------------------------- */
/* cgipaf.c                                     (GPL) 2000,2001 Belgium    */
/* http://stafwag.netfirms.com                          Staf Wagemakers    */
/* staf.wagemakers@advalvas.be                                             */
/* ----------------------------------------------------------------------- */

#include "cgipaf_defs.h"

main()
{
#include "cgipaf_vars.h"

unsetenv("IFS");
setuid(0);
set_memerr(out_of_memory);

/*
 * set doc_root & accessdb
 */

/* Where are my documents??? */
if ((config_file=fopen(CONFIGFILE,"r"))!=NULL) {
   doc_root=get_sg_item(config_file,CFGSECTION,DOC_ROOT);
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_SYSLOG))!=NULL) {
      if (!strcasecmp(cp,"off")) enable_log(0);
      free(cp);
   }
   if ((cp=get_config_item(config_file,CFG_LOGLEVEL))!=NULL) { 
      sscanf(cp,"%d",&brol);
      set_loglevel(brol);
      free(cp);
      write_log(LOG_USER,7,"Set loglevel to %d",set_loglevel(-1));
   }
}

if (config_file!=NULL) write_log(LOG_USER,7,"configfile %s opened",CONFIGFILE);
   else {
      write_log(LOG_USER,1,"can't open configfile %s, %s",CONFIGFILE,strerror(errno));
      set_loglevel(7);
      write_log(LOG_USER,7,"failed to open configfile, switch to debug mode");
   }
   
if (doc_root!=NULL) {
   write_log(LOG_USER,7,"doc_root set to %s",doc_root);
   if (chdir(doc_root)==-1) write_log(LOG_USER,3,"chdir(%s) failed, %s",doc_root,strerror(errno));
}
#ifdef _WITHPAM
if ((pam_servicename=get_sg_item(config_file,CFGSECTION,CFG_PAM_SERVICE))!=NULL) {
   set_pam_service(pam_servicename);
}
   pam_servicename=set_pam_service(NULL);
   write_log(LOG_USER,7,"pam service name set to %s",pam_servicename);
#ifdef CGIPAF_PASSWD

if ((cp=get_sg_item(config_file,CFGSECTION,CFG_PAM_CHANGE_EXPIRED_AUTHTOK))!=NULL) {
   if (!strcasecmp(cp,"off")) {
      write_log(LOG_USER,7,"set PAM_CHANGE_EXPIRED_AUTHOK to off");
      set_pam_chauth_flag(0);
   }
   else {   
      write_log(LOG_USER,7,"set PAM_CHANGE_EXPIRED_AUTHOK to on");
   }
   free(cp);
}

#endif
   
#endif

#ifdef HAVE_LIBCRACK
if ((cp=get_sg_item(config_file,CFGSECTION,CFG_CRACKLIB))!=NULL) {
      if (!strcasecmp(cp,"off")) {
	 enable_cracklib=0;
         write_log(LOG_USER,7,"cracklib disabled");
      }
      else {
	 enable_cracklib=1;
	 write_log(LOG_USER,7,"cracklib enabled");
      }
      free(cp);
}
#endif
/*
 * Should we use an accessdb, if yes get max_invalid & invalid_timeout
 */
  
if ((cp=get_sg_item(config_file,CFGSECTION,CFG_ACCESSDB))!=NULL) {
   accessdb=cp;
   write_log(LOG_USER,7,"accessdb set to %s",accessdb);
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_MAXINVALID))!=NULL) {
      sscanf(cp,"%d",&max_invalid);
      write_log(LOG_USER,7,"max_invalid set to %d",max_invalid);
      }
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_INVALIDTIMEOUT))!=NULL) {
      sscanf(cp,"%d",&invalid_timeout);
      write_log(LOG_USER,7,"invalid_timeout set to %d",invalid_timeout);
      }
   }
   snprintf(invalid_timeout_txt,80,"%d",invalid_timeout);
   snprintf(max_invalid_txt,80,"%d",max_invalid);      
#ifdef CGIPAF_VIEWMAILCFG
   if(config_file==NULL) {
     print_txt_msg(err_mcfg_configfile);
     puts(CONFIGFILE);
     puts(txt_contact_webmaster);
     write_log(LOG_USER,3,"%s %s",err_mcfg_configfile,CONFIGFILE);
     exit(0);
   }
   if(accessdb==NULL) {
     write_log(LOG_USER,3,"%s",err_mcfg_accessdb);
     show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESSDB,err_mcfg_accessdb,options);
     exit(0);
   }
cp=get_section_config_item(config_file,CFGSECTION,CFG_COOKIETIMEOUT);
if(cp!=NULL) sscanf(cp,"%d",&cookie_timeout);

write_log(LOG_USER,7,"cookie_timeout set to %d",cookie_timeout);
   
snprintf(cookie_timeout_txt,80,"%d",cookie_timeout);

#endif
#ifdef CGIPAF_PASSWD

/* 
 * Get the max and min password length and store them in 
 * min_length / max_length
 */
   
if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MINLENGTH))==NULL) min_length=MINLENGTH;
   else { sscanf(cp,"%d",&min_length); }
   write_log(LOG_USER,7,"min_length set to %d",min_length);
   snprintf(min_length_txt,80,"%d",min_length);

if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MAXLENGTH))==NULL) max_length=MAXLENGTH;
   else { sscanf(cp,"%d",&max_length); }
   write_log(LOG_USER,7,"min_length set to %d",max_length);
   snprintf(max_length_txt,80,"%d",max_length);

/* We dont want too much data */
   
   if ((cp=getenv("CONTENT_LENGTH"))!=NULL) {
   sscanf(cp,"%d",&brol);
   if (brol>150) {
      print_html_msg(txt_too_much);
      write_log(LOG_USER,3,"%s",txt_too_much);
      exit(1);
      }
   }
#endif

/*
 * get post data
 * set name & pass
 */ 

/* Get the post variables */
   
data=read_post();

/* No Post? -> Ask the user to login */
   
if (!data) {
   write_log(LOG_USER,7,"no POST data, show login_document");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,LOGIN_DOCUMENT,err_readdata,options);
   }
/* If there is no loginname ask for it */
   
if (!(cp=get_postitem(data,LOGIN))) {
   write_log(LOG_USER,7,"name not set, show err_login");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options);
   }
name=textarea2asc(cp);
free(cp);
/* Make the login available to a redirect / ePHP */
   
if (name!=NULL) {
   write_log(LOG_USER,7,"name set to %s",name);
   options[0][1]=name;
}

/* We dont like too short loginnames */

if (strlen(name)<1) {
   write_log(LOG_USER,7,"%s is invalid strlen(name)=0",name);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options);
   }

/* root = god, god should know how to change his password */
   
if (!strcmp(name,ROOT)) {
   write_log(LOG_AUTHPRIV,5,warn_root);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }

/* no password = no login */

if (!(cp=get_postitem(data,PASSWORD))) {
   write_log(LOG_USER,7,"user forgot to type his password");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   }

pass=textarea2asc(cp);
free(cp);

#ifdef CGIPAF_PASSWD
/* get the new password or die */

if (!(cp=get_postitem(data,NEWPASS1))) {
   write_log(LOG_USER,7,"newpass1 not set");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options);
   }
newpass1=textarea2asc(cp);
free(cp);
if (!(cp=get_postitem(data,NEWPASS2))) {
   write_log(LOG_USER,7,"newpass2 not set");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options);
   }
newpass2=textarea2asc(cp);
free(cp);
/* match or die */
if (strcmp(newpass1,newpass2)) {
   write_log(LOG_USER,7,"new passwords don't match");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_MATCH,err_match,options);
   }

/* nothing to do... */
if (strcmp(newpass1,pass)==0) {
   write_log(LOG_USER,7,"password unchanged");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UNCHANGED,err_unchanged,options);
   }

/* we dont like too short passwords */
if (strlen(newpass1)<min_length) {
   write_log(LOG_USER,7,"new password too short");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_TOOSHORT,err_tooshort,options);
   }

/* 
 * we dont like too long passwords, users cant remind too long passwords and
 * call while we're sleeping...
 */
   
if (strlen(newpass2)>max_length) {
   write_log(LOG_USER,7,"new password too long");
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_TOOLONG,err_toolong,options);
   }

#endif

/*
 * test pass, and create accessdb
 */
/* fetch the user data, if the user dont exist... dont tell them */
   
if (!(pw=get_pw(name))) {
   write_log(LOG_USER,7,"get_pw(%s) failed, user %s doesn't exists",name,name);
   write_log(LOG_AUTHPRIV,6,"Invalid password for user %s",name);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   };
   
/* get the min uid, and compare it with the real uid if uid<min_uid die */
   
if ((cp=get_config_item(config_file,CFG_MINUID))==NULL) brol=MINUID;
   else { sscanf(cp,"%d",&brol);
          if (brol<MINUID) brol=MINUID;
   }
   write_log(LOG_USER,7,"min_uid set to %d",brol);
if (pw->p->pw_uid<brol) {
   write_log(LOG_AUTHPRIV,5," %s %d %s",warn_uid,pw->p->pw_uid,warn_below_minuid);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }

/* get the max uid, and compare it with the real uid uid>max_uid die */
   
if((cp=get_config_item(config_file,CFG_MAXUID))!=NULL) {
   int maxuid=brol;
   sscanf(cp,"%d",&brol);
   if (brol>maxuid) maxuid=brol;
   write_log(LOG_USER,7,"max_uid set to %d",maxuid);
   if (pw->p->pw_uid>maxuid) {
      write_log(LOG_AUTHPRIV,5,"%s %d %s",warn_uid,pw->p->pw_uid,warn_above_maxuid);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }
}
if (accessdb) {
   if ((brol=get_access_status(accessdb,name,max_invalid,invalid_timeout))>0) {
      snprintf(invalid_wait_txt,80,"%d",brol);
      show_msg(config_file,doc_root,CFGSECTION,ERR_LOCKED,err_locked,options);
      i=run_cmd(config_file,CFGSECTION,RUN_LOCKED,options);
      if(i==-1) 
	write_log(LOG_USER,1,"Can't execute run_locked %s",strerror(errno));
      if(config_file!=NULL) fclose(config_file);
      write_log(LOG_AUTHPRIV,5,"User %s is locked",name);
      exit(0);
   }
}
if ((i=ckpw(pw,pass))!=PASS_SUCCESS) {
   write_log(LOG_USER,7,"ckpw() failed, probably wrong password");
#ifdef _WITHPAM
   write_log(LOG_USER,7,"ckpw() failed, pam error = %s",pam_strerror(pw->pamh,i));
#endif
   show_msg(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   write_log(LOG_AUTHPRIV,6,"Invalid password for user %s",name);
   if (accessdb) {
      if (save_access_status(accessdb,name,1,invalid_timeout,cookie)==-1) {
	 printf("%s %s",warn_update_accessdb,accessdb);
	 write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
      }
   }

   if (config_file!=NULL) fclose(config_file);   
   exit(0);
}

#ifdef CGIPAF_PASSWD

#ifdef HAVE_LIBCRACK
if(enable_cracklib) {
if ((cp=get_sg_item(config_file,CFGSECTION,CFG_CRACKLIB_DICTPATH))!=NULL) {
   cracklib_dictpath=xmalloc(strlen(cp)+1);
   strcpy(cracklib_dictpath,cp);
   free(cp);
   write_log(LOG_INFO,7,"cracklib_dictpath set to \"%s\"",cracklib_dictpath);
}
if (cracklib_dictpath!=NULL) {
    char const * const crack_msg = FascistCheck(newpass1,cracklib_dictpath);
    if (0 != crack_msg) {
       options[16][1]=(char *) xmalloc(strlen(crack_msg)+1);
       strcpy(options[16][1],crack_msg);
       write_log(LOG_USER,7,"%s %s",err_cracklib,options[16][1]);
       if(show_msg(config_file,doc_root,CFGSECTION,ERR_CRACKLIB,err_cracklib,options)==2) printf("%s\n",options[16][1]);
       if (config_file!=NULL) fclose(config_file);   
       exit(0);
       }
    }
    else {
    write_log(LOG_INFO,4,"cracklib_dictpath not set, cracklib is disabled");
    }
}
#endif

if ((brol=chpw(pw,newpass1))==PASS_SUCCESS) {
   show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_changed,options);
   fflush(stdout);
   write_log(LOG_AUTHPRIV,6,"Password for %s was changed",name);
   options[15][1]=newpass1;
   i=run_cmd(config_file,CFGSECTION,RUN_SUCCESS,options);
   options[15][1]=txt_NULL;
   if(i==-1) 
      write_log(LOG_AUTHPRIV,1,"Can't execute run_success %s",strerror(errno)); 
   if (accessdb) {
      if (save_access_status(accessdb,name,0,invalid_timeout,NULL)==-1) {
	 printf("%s %s",warn_update_accessdb,accessdb);
	 write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
      }
   }
}
else {
#ifdef _WITHPAM
   if(last_pam_msg()!=NULL) {
     options[17][1]=last_pam_msg();
     if(show_msg(config_file,doc_root,CFGSECTION,ERR_PAMERROR,NULL,options)<3) {
     write_log(LOG_AUTHPRIV,1,"pam err %s",options[17][1]);
       if (config_file!=NULL) fclose(config_file);   
       exit(0);
     }
   }
     i=brol;
     brol=-15;
#endif
           print_html_msg("<CENTER><H1><B><I>");
           printf("<BR>Can't update password<BR>%s",err_chpw[abs(brol)-1]);
           if (errno) printf(", %s<BR>",strerror(errno));
#ifdef _WITHPAM
           if(last_pam_msg()!=NULL) printf(", %s<BR>",last_pam_msg());
           else printf(", %s<BR>",pam_strerror(pw->pamh,i));
#endif
	   printf("</I></B></H1></CENTER>");
           write_log(LOG_AUTHPRIV,1,"Can't update password %s",err_chpw[abs(brol)-1]);
#ifdef _WITHPAM
           write_log(LOG_AUTHPRIV,1,"pam error %s",pam_strerror(pw->pamh,i));
#endif
	   }
#endif
#ifdef CGIPAF_VIEWMAILCFG
cookie=create_cookie();
if(cookie==NULL) {
  print_txt_msg("Can't create cookie");
  write_log(LOG_AUTHPRIV,1,"create_cookie() failed");
  exit(0);
}
set_cookie("cgipaf",cookie,"path=/");
if (save_access_status(accessdb,name,0,invalid_timeout,cookie)==-1) {
    print_txt_msg("Warning: failed to update");puts(accessdb);
    write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
}
write_log(LOG_USER,7,"set uid to %d",pw->p->pw_uid);
if (setgid(pw->p->pw_gid)==-1) {
   write_log(LOG_USER,7,"setgid() failed");
};
write_log(LOG_USER,7,"set gid to %d",pw->p->pw_gid);
if (setuid(pw->p->pw_uid)==-1) {
   write_log(LOG_USER,7,"setuid() failed");
}
write_log(LOG_USER,7,"set umask to 0177");
umask(0177);

write_log(LOG_USER,7,"reading current mail configuration....");
if((forward_to=get_kforward(pw))!=NULL) {
   strcpy(forward,"yes"); strcpy(keep_msg,"yes");
   write_log(LOG_USER,7,"forwarding with keepmsg is enabled in current cfg, mail are forwarded to %s",forward_to);
}
   
else {
     if ((forward_to=get_forward(pw))!=NULL) {
	strcpy(forward,"yes");
	write_log(LOG_USER,7,"forwarding is enabled in current cfg, mail are forwarded to %s",forward_to);
     }
	 
     else {
	forward_to=txt_NULL;
	write_log(LOG_USER,7,"mailforwarding is not enabeled");
     }
};
options[6][1]=forward_to;
  

if (get_reply(pw)) {
   strcpy(autoreply,"yes");
   write_log(LOG_USER,7,"autoreply is enabled in the current cfg");
   }
   else write_log(LOG_USER,7,"autoreply is not enabled");
autoreply_msg=get_vacations(pw);
if (autoreply_msg!=NULL) options[13][1]=autoreply_msg;

if (strcmp(forward,"no")) strcpy(not_forward,"no");
if (strcmp(keep_msg,"no")) strcpy(not_keep_msg,"no");
if (strcmp(autoreply,"no")) strcpy(not_autoreply,"no");
write_log(LOG_USER,7,"got the mailconfiguration for user %s",name);

   if(show_msg(config_file,doc_root,CFGSECTION,CFG_MAILCFG,NULL,options)==3) {
write_log(LOG_USER,7,"couldn't load mailcfg_document using default");
print_html_msg("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
printf("<HTML>\n");
printf("<HEAD>\n");
printf("<TITLE>Change your mailbox configuration</TITLE>");
printf("</HEAD>\n");
printf("<BODY text=\"#000000\" bgcolor=\"#B0B0B0\">\n");
printf("<H1>Change your mailbox configuration</H1>\n");
printf("<HR>\n");
printf("<P>\n");
printf("<FORM ACTION=\"/cgi-bin/mailcfg.cgi\" METHOD=POST>\n");
printf("<TABLE WIDTH=100%>\n");
printf("<TR>\n");
printf("<TD>Login name: %s</TD>\n",name);
printf("<TD ALIGN=\"LEFT\"><INPUT NAME=name VALUE=\"%s\" TYPE=HIDDEN SIZE=10 MAXLENGTH=15></TD>\n",name);
printf("</TR>\n");
printf("</TABLE>\n");
printf("<HR>\n");
printf("<TABLE>\n");
printf("<TR>\n");
printf("<TD>Forward mailbox:</TD>\n");
printf("<TD WIDTH=20>\n");
printf("<SELECT NAME=\"forward\">\n");
printf("<OPTION value=\"%s\">%s\n",forward,forward);
printf("<OPTION value=\"%s\">%s\n",not_forward,not_forward);
printf("</SELECT></TD>\n");
printf("<TD>Forward to:</TD>\n");
printf("<TD><INPUT NAME=forward_to VALUE=\"%s\" TYPE=TEXT SIZE=30 MAXLENGTH=40></TD>\n",forward_to);
printf("</TR>\n");
printf("<TR>\n");
printf("<TD>Keep messages:</TD>\n");
printf("<TD>\n");
printf("<SELECT NAME=\"keep_msg\">\n");
printf("<OPTION value=\"%s\">%s\n",keep_msg,keep_msg);
printf("<OPTION value=\"%s\">%s\n",not_keep_msg,not_keep_msg);
printf("</SELECT><TD>\n");
printf("</TR>\n");
printf("</TABLE>\n");
printf("<HR>\n");
printf("<TABLE>\n");
printf("<TR>\n");
printf("<TD>Enable autoReply:</TD>\n");
printf("<TD>\n");
printf("<SELECT NAME=\"autoreply\">\n");
printf("<OPTION value=\"%s\">%s\n",autoreply,autoreply);
printf("<OPTION value=\"%s\">%s\n",not_autoreply,not_autoreply);
printf("</SELECT>\n");
printf("</TD>\n");
printf("</TABLE>\n");
printf("Autoreply message:<BR>\n");
printf("<TEXTAREA NAME=\"autoreply_msg\" ROWS=10 COLS=60>");
printf("%s",options[13][1]); 
printf("</TEXTAREA>\n");
printf("<P>\n");
printf("<CENTER>\n");
printf("<TABLE>\n");
printf("<TR>\n");
printf("<TD>\n");
printf("<INPUT TYPE=RESET VALUE=\"Clear\">\n");
printf("<INPUT TYPE=SUBMIT VALUE=\"Submit\">\n");
printf("</TD></TR></TABLE>\n");
printf("</CENTER>\n");
printf("</FORM>");
printf("</BODY>");   
fflush(stdout);   
}
write_log(LOG_AUTHPRIV,6,"User %s has login successfully",name);
#endif
if (config_file != NULL) fclose(config_file);
exit(0);
}
