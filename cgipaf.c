/* ----------------------------------------------------------------------- */
/* cgipaf.c                                             (GPL) 2000 Belgium */
/* http://www.stafwag.f2s.com                           Staf Wagemakers    */
/*                                                      stafwag@f2s.com    */
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
}


if (doc_root!=NULL) chdir(doc_root);

/*
 * Should we use an accessdb, if yes get max_invalid & invalid_timeout
 */
  
if ((cp=get_sg_item(config_file,CFGSECTION,CFG_ACCESSDB))!=NULL) {
   accessdb=cp;
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_MAXINVALID))!=NULL) {
      sscanf(cp,"%d",&max_invalid);
      }
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_INVALIDTIMEOUT))!=NULL) {
      sscanf(cp,"%d",&invalid_timeout);
      }
   }
   snprintf(invalid_timeout_txt,80,"%d",invalid_timeout);
   snprintf(max_invalid_txt,80,"%d",max_invalid);      
#ifdef CGIPAF_VIEWMAILCFG
   if(config_file==NULL) {
     print_txt_msg(err_mcfg_configfile);
     puts(CONFIGFILE);
     puts(txt_contact_webmaster);
     write_log(LOG_USER,LOG_ERR,"%s %s",err_mcfg_configfile,CONFIGFILE);
     exit(0);
   }
   if(accessdb==NULL) {
     write_log(LOG_USER,LOG_ERR,"%s",err_mcfg_accessdb);
     show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESSDB,err_mcfg_accessdb,options);
     exit(0);
   }
cp=get_section_config_item(config_file,CFGSECTION,CFG_COOKIETIMEOUT);
if(cp!=NULL) sscanf(cp,"%d",&cookie_timeout);

snprintf(cookie_timeout_txt,80,"%d",cookie_timeout);

#endif
#ifdef CGIPAF_PASSWD

/* 
 * Get the max and min password length and store them in 
 * min_length / max_length
 */
   
if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MINLENGTH))==NULL) min_length=MINLENGTH;
   else { sscanf(cp,"%d",&min_length); }
   snprintf(min_length_txt,80,"%d",min_length);

if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MAXLENGTH))==NULL) max_length=MAXLENGTH;
   else { sscanf(cp,"%d",&max_length); }
   snprintf(max_length_txt,80,"%d",max_length);

/* We dont want too much data */
   
   if ((cp=getenv("CONTENT_LENGTH"))!=NULL) {
   sscanf(cp,"%d",&brol);
   if (brol>100) {
      print_html_msg(txt_too_much);
      write_log(LOG_USER,LOG_ERR,"%s",txt_too_much);
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
   show_msg_and_exit(config_file,doc_root,CFGSECTION,LOGIN_DOCUMENT,err_readdata,options);
   }
/* If there is no loginname ask for it */
   
if (!(name=get_postitem(data,LOGIN))) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options);
   }

/* Make the login available to a redirect / ePHP */
   
if (name!=NULL) options[0][1]=name;

/* We dont like too short loginnames */

if (strlen(name)<1) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options);
   }

/* root = god, god should know how to change his password */
   
if (!strcmp(name,ROOT)) {
   write_log(LOG_AUTHPRIV,LOG_NOTICE,warn_root);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }

/* no password = no login */

if (!(pass=get_postitem(data,PASSWORD))) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   }

#ifdef CGIPAF_PASSWD
/* get the new password or die */

if (!(newpass1=get_postitem(data,NEWPASS1))) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options);
   }
if (!(newpass2=get_postitem(data,NEWPASS2))) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options);
   }
/* match or die */
if (strcmp(newpass1,newpass2)) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_MATCH,err_match,options);
   }
/* we dont like too short passwords */
if (strlen(newpass1)<min_length) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_TOOSHORT,err_tooshort,options);
   }

/* 
 * we dont like too long passwords, users cant remind too long passwords and
 * call while we're sleeping...
 */
   
if (strlen(newpass2)>max_length) {
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_TOOLONG,err_toolong,options);
   }

#endif

/*
 * test pass, and create accessdb
 */
/* fetch the user data, if the user dont exist... dont tell them */
   
if (!(pw=get_pw(name))) {
   write_log(LOG_AUTHPRIV,LOG_INFO,"Invalid password for user %s",name);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   };
   
/* get the min uid, and compare it with the real uid if uid<min_uid die */
   
if ((cp=get_config_item(config_file,CFG_MINUID))==NULL) brol=MINUID;
   else { sscanf(cp,"%d",&brol);
          if (brol<MINUID) brol=MINUID;
   }
if (pw->p->pw_uid<brol) {
   write_log(LOG_AUTHPRIV,LOG_NOTICE," %s %d %s",warn_uid,pw->p->pw_uid,warn_below_minuid);
   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }

/* get the max uid, and compare it with the real uid uid>max_uid die */
   
if((cp=get_config_item(config_file,CFG_MAXUID))!=NULL) {
   int maxuid=brol;
   sscanf(cp,"%d",&brol);
   if (brol>maxuid) maxuid=brol;
   if (pw->p->pw_uid>maxuid) {
      write_log(LOG_AUTHPRIV,LOG_NOTICE,"%s %d %s",warn_uid,pw->p->pw_uid,warn_above_maxuid);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options);
   }
}
if (accessdb) {
   if ((brol=get_access_status(accessdb,name,max_invalid,invalid_timeout))>0) {
      snprintf(invalid_wait_txt,80,"%d",brol);
      show_msg(config_file,doc_root,CFGSECTION,ERR_LOCKED,err_locked,options);
      i=run_cmd(config_file,CFGSECTION,RUN_LOCKED,options);
      if(i==-1) 
	write_log(LOG_USER,LOG_ALERT,"Can't execute run_locked %s",strerror(errno));
      if(config_file!=NULL) fclose(config_file);
      write_log(LOG_AUTHPRIV,LOG_NOTICE,"User %s is locked",name);
      exit(0);
   }
}
if (ckpw(pw,pass)==-1) {
   show_msg(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   write_log(LOG_AUTHPRIV,LOG_INFO,"Invalid password for user %s",name);
   if (accessdb) {
      if (save_access_status(accessdb,name,1,invalid_timeout,cookie)) {
	 printf("%s %s",warn_update_accessdb,accessdb);
	 write_log(LOG_AUTHPRIV,LOG_ALERT,"%s %s",warn_update_accessdb,accessdb);
      }
   }

   if (config_file!=NULL) fclose(config_file);   
   exit(0);
   }

#ifdef CGIPAF_PASSWD
if ((brol=chpw(pw,newpass1))==0) {
   show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_changed,options);
   fflush(stdout);
   write_log(LOG_AUTHPRIV,LOG_INFO,"Password for %s was changed",name);
   options[15][1]=newpass1;
   i=run_cmd(config_file,CFGSECTION,RUN_SUCCESS,options);
   options[15][1]=txt_NULL;
   if(i==-1) 
      write_log(LOG_AUTHPRIV,LOG_ALERT,"Can't execute run_success %s",strerror(errno)); 
   if (accessdb) {
      if (save_access_status(accessdb,name,0,invalid_timeout,NULL)) {
	 printf("%s %s",warn_update_accessdb,accessdb);
	 write_log(LOG_AUTHPRIV,LOG_ALERT,"%s %s",warn_update_accessdb,accessdb);
      }
   }
}
else {
           print_html_msg("<CENTER><H1><B><I>");
           printf("<BR>Can't update password<BR><BR>%s",err_chpw[abs(brol)-1]);
           if (errno) printf(", %s<BR>",strerror(errno));
	   printf("</I></B></H1></CENTER>");
	   write_log(LOG_AUTHPRIV,LOG_ALERT,"Can't update password %s",err_chpw[abs(brol)-1]);
	   }
#endif
#ifdef CGIPAF_VIEWMAILCFG
cookie=create_cookie();
if(cookie==NULL) {
  print_txt_msg("Can't create cookie");
  write_log(LOG_AUTHPRIV,LOG_ALERT,"create_cookie() failed");
  exit(0);
}
set_cookie("cgipaf",cookie,"path=/");
if (save_access_status(accessdb,name,0,invalid_timeout,cookie)) {
    print_txt_msg("Warning: failed to update");puts(accessdb);
    write_log(LOG_AUTHPRIV,LOG_ALERT,"%s %s",warn_update_accessdb,accessdb);
}
setuid(pw->p->pw_uid);
setgid(pw->p->pw_gid);
umask(0177);
forward_to[0]='\0';

if(get_kforward(pw,forward_to)) {strcpy(forward,"yes"); strcpy(keep_msg,"yes");}
  else if ((get_forward(pw,forward_to))) strcpy(forward,"yes");

if (get_reply(pw)) {strcpy(autoreply,"yes");}
autoreply_msg=get_vacations(pw);
if (autoreply_msg!=NULL) options[13][1]=autoreply_msg;

if (strcmp(forward,"no")) strcpy(not_forward,"no");
if (strcmp(keep_msg,"no")) strcpy(not_keep_msg,"no");
if (strcmp(autoreply,"no")) strcpy(not_autoreply,"no");

if(show_msg(config_file,doc_root,CFGSECTION,CFG_MAILCFG,NULL,options)==3) {
print_html_msg("<!DOCTYPE HTML PUBLIC \"-//IETH/DTD HTML 3.0//EN\">\n");
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
printf("<TD>Login name:</TD>\n");
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
printf("</TABLE>\n");
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
write_log(LOG_AUTHPRIV,LOG_INFO,"User %s has login successfully");
   
}
#endif
if (config_file != NULL) fclose(config_file);
exit(0);
}
