/*
 *
 * cgiPAF.c     		
 *
 *
 * Copyright (C) 2000-03 Staf Wagemakers Belgie/Belgium 
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */


#include "cgipaf_defs.h"

main()
{
#include "cgipaf_vars.h"
#include "cgipaf_init.c"

   /* If there is no loginname ask for it */
   
   if (!(cp=get_postitem(data,LOGIN))) {
      write_log(LOG_USER,7,"name not set, show err_login");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options,txt_message);
   }
   name=textarea2asc(cp);
   /* xfree(cp); */			/* keep the orginal post array */
   
   /* Make the login available to a redirect / ePHP */
   
   if (name!=NULL) {
      write_log(LOG_USER,7,"name set to %s",name);
      options[0][1]=xmalloc(strlen(name)+1);
      strcpy(options[0][1],name);
   }

   /* We dont like too short loginnames */
   
   if (strlen(name)<1) {
      write_log(LOG_USER,7,"%s is invalid strlen(name)=0",name);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options,txt_message);
   }

   /* root = god, god should know how to change his password */
   
   if (!strcmp(name,ROOT)) {
      write_log(LOG_AUTHPRIV,5,warn_root);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);
   }

   /* no password = no login */

   if (!(cp=get_postitem(data,PASSWORD))) {
      write_log(LOG_USER,7,"user forgot to type his password");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
   }

   pass=textarea2asc(cp);
   
   /* free(cp); */			/* keep the orginal postarray */

#ifdef CGIPAF_PASSWD
   
/* get the new password or die */

   if (!(cp=get_postitem(data,NEWPASS1))) {
      write_log(LOG_USER,7,"newpass1 not set");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options,txt_message);
   }
   newpass1=textarea2asc(cp);
   /* free(cp); */			/* keep the orginal postarray */
   if (!(cp=get_postitem(data,NEWPASS2))) {
      write_log(LOG_USER,7,"newpass2 not set");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options,txt_message);
   }
   newpass2=textarea2asc(cp);
   /* free(cp); */			/* keep the orginal postarray */
   
   /* match or die */
   
   if (strcmp(newpass1,newpass2)) {
      write_log(LOG_USER,7,"new passwords don't match");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_MATCH,err_match,options,txt_message);
   }

   /* nothing to do... */
   
   if (strcmp(newpass1,pass)==0) {
      write_log(LOG_USER,7,"password unchanged");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UNCHANGED,err_unchanged,options,txt_message);
   }

   /* we dont like too short passwords */
   
   if (strlen(newpass1)<min_length) {
      write_log(LOG_USER,7,"new password too short");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_TOOSHORT,err_tooshort,options,txt_message);
   }

   /* 
    * we dont like too long passwords, users cant remind too long 
    * passwords and call while we're sleeping...
    */
   
   if (strlen(newpass2)>max_length) {
      write_log(LOG_USER,7,"new password too long");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_TOOLONG,err_toolong,options,txt_message);
   }

   /*
    * Make sure password does not contain an illegal word
    * Added by Daniel R.
    */
    ccp=illegal_words;

    /* We have 2 copies of the password, I am going to make one lowercase for checking purposes */
    if ((strtolower(newpass2) == -1))  {
       write_log(LOG_USER,7,"strlolower(newpass2) failed");
       print_txt_msg("Error making password lower case\n");
    }
    else {
       if ((ccp!=NULL)&&(*ccp!=NULL)) {
          while (*ccp != NULL) { /* end of list will be *ccp == NULL */
	     if (**ccp != '\0' && (strtolower(*ccp) !=-1)) {
		/* Empty word entry will be **ccp == \0 */
		if (strstr(newpass2, *ccp)) {
		   options[21][1] = *ccp; 
                   write_log(LOG_USER,7,"new passwords contains an illegal word %s",options[21][1]);
		   show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_BADWORD,err_badword,options,txt_message);
		}
	     }
	     ccp++;
	  }
       }

       /* they can not use their login name for a password */
       if (strcmp(newpass2, name) == 0) {
       options[21][1] = name;
       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_BADWORD,err_badword,options,txt_message);
       }
    }

#endif


    #include "cgipaf_pwlocation.c"

   /*
    * test pass, and create accessdb
    */
   
   /* fetch the user data, if the user dont exist... dont tell them */
   
   if (!(pw=get_pw(name))) {
      write_log(LOG_USER,7,"get_pw(%s) failed, user %s doesn't exists",name,name);
      write_log(LOG_AUTHPRIV,6,"Invalid password for user %s",name);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
   };
   write_log(LOG_USER,7,"pw->p->pwuid = %d",pw->p->pw_uid);

#ifdef _WITHPAM
#ifdef CGIPAF_PASSWD
  
   /*
    * Set the real uid to the user's uid, set the effective uid to root
    */
   
   if(setreuid(pw->p->pw_uid,0)==-1) {
        write_log(LOG_USER,7,"setreuid() failed: %s",strerror(errno));
   }; 

#endif
#endif

   
   /* include access control lists */
   
   #include "cgipaf_acl.c" 
   
   /* get the min uid, and compare it with the real uid if uid<min_uid die */
   
   if ((cp=get_config_item(config_file,CFG_MINUID))==NULL) brol=MINUID;
   else { sscanf(cp,"%d",&brol);
      if (brol<MINUID) brol=MINUID;
   }
   write_log(LOG_USER,7,"min_uid set to %d",brol);
   if (pw->p->pw_uid<brol) {
      write_log(LOG_AUTHPRIV,5," %s %d %s",warn_uid,pw->p->pw_uid,warn_below_minuid);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);
   }

   /* get the max uid, and compare it with the real uid uid>max_uid die */
   
   if((cp=get_config_item(config_file,CFG_MAXUID))!=NULL) {
      int maxuid=brol;
      sscanf(cp,"%d",&brol);
      if (brol>maxuid) maxuid=brol;
      write_log(LOG_USER,7,"max_uid set to %d",maxuid);
      if (pw->p->pw_uid>maxuid) {
	 write_log(LOG_AUTHPRIV,5,"%s %d %s",warn_uid,pw->p->pw_uid,warn_above_maxuid);
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);
      }
   }
   
   /* lock the user if he has too many invalid logins */
   
   if (accessdb) {
      if ((brol=get_access_status(accessdb,options[0][1],max_invalid,invalid_timeout))>0) {
	 snprintf(invalid_wait_txt,80,"%d",brol);
	 show_msg(config_file,doc_root,CFGSECTION,ERR_LOCKED,err_locked,options,txt_message);
	 i=run_cmd(config_file,CFGSECTION,RUN_LOCKED,options);
	 if(i<0) {
	    if(i==-1) 
	      write_log(LOG_USER,1,"Can't execute run_locked %s run_cmd() failed",strerror(errno));
	 }
	 else
	   if((i=WEXITSTATUS(i)))
	     write_log(LOG_USER,1,"run_locked returns a non-null exit code %d",i);
	 if(config_file!=NULL) fclose(config_file);
	 write_log(LOG_AUTHPRIV,5,"User %s is locked",options[0][1]);
	 exit(0);
      }
   }
   
   /* invalid password handling */
   
   if ((i=ckpw(pw,pass))!=PASS_SUCCESS) {
      write_log(LOG_USER,7,"ckpw() failed, probably wrong password");
#ifdef _WITHPAM
      write_log(LOG_USER,7,"ckpw() failed, pam error = %s",pam_strerror(pw->pamh,i));
#endif
      show_msg(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
      write_log(LOG_AUTHPRIV,6,"Invalid password for user %s",name);
      options[19][1]=pw->p->pw_dir;
      if (accessdb) {
	 if (save_access_status(accessdb,options[0][1],1,invalid_timeout,cookie)==-1) {
	    printf("%s %s",warn_update_accessdb,accessdb);
	    write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
	 }
      }

      if (config_file!=NULL) fclose(config_file);   
      exit(0);
   }
   options[19][1]=pw->p->pw_dir;

#ifdef CGIPAF_PASSWD

#ifdef HAVE_LIBCRACK
   
   /* test the new password if cracklib is enabled */
   
   if(enable_cracklib) {
      if ((cp=get_sg_item(config_file,CFGSECTION,CFG_CRACKLIB_DICTPATH))!=NULL) {
	 cracklib_dictpath=xmalloc(strlen(cp)+1);
	 strcpy(cracklib_dictpath,cp);
	 xfree(cp);
	 write_log(LOG_INFO,7,"cracklib_dictpath set to \"%s\"",cracklib_dictpath);
      }
      if (cracklib_dictpath!=NULL) {
	 char const * const crack_msg = FascistCheck(newpass1,cracklib_dictpath);
	 if (0 != crack_msg) {
	    options[16][1]=(char *) xmalloc(strlen(crack_msg)+1);
	    strcpy(options[16][1],crack_msg);
	    options[21][1]=options[16][1];
	    write_log(LOG_USER,7,"%s %s",err_cracklib,options[16][1]);
	    if(show_msg(config_file,doc_root,CFGSECTION,ERR_CRACKLIB,err_cracklib,options,txt_message)==2) printf("%s\n",options[16][1]);
	    if (config_file!=NULL) fclose(config_file);   
	    exit(0);
	 }
      }
      else {
	 write_log(LOG_INFO,4,"cracklib_dictpath not set, cracklib is disabled");
      }
   }

#endif /* HAVE_CRACKLIB */

   /* finally one use how can remmber his password? */
   
   if ((brol=chpw(pw,newpass1))==PASS_SUCCESS) {
      show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_changed,options,txt_message);
      fflush(stdout);
      write_log(LOG_AUTHPRIV,6,"Password for %s was changed",name);
      options[15][1]=newpass1;

      /*
       * reset uid to run the run_success script
       */

      if(setuid(0)==-1) {
        write_log(LOG_USER,7,"setuid(0) failed: %s",strerror(errno));
      };

      i=run_cmd(config_file,CFGSECTION,RUN_SUCCESS,options);
      options[15][1]=txt_NULL;
      if(i<0) {
	 if(i==-1) 
	   write_log(LOG_AUTHPRIV,1,"Can't execute run_success %s run_cmd() failed",strerror(errno));
      }
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_AUTHPRIV,1,"run_success returns a non-null exit code %d",i); 
      if (accessdb) {
	 if (save_access_status(accessdb,options[0][1],0,invalid_timeout,NULL)==-1) {
	    printf("%s %s",warn_update_accessdb,accessdb);
	    write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
	 }
      }
   }
   else {
      
      /* use cant remember his pw, tell them... */
      
#ifdef _WITHPAM
      if(last_pam_msg()!=NULL) {
	 options[17][1]=last_pam_msg();
	 if(show_msg(config_file,doc_root,CFGSECTION,ERR_PAMERROR,NULL,options,txt_message)<3) {
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
   
   /* make a cookie and give it to the use keep them sweet... */
   
   cookie=create_cookie();
   if(cookie==NULL) {
      print_txt_msg("Can't create cookie");
      write_log(LOG_AUTHPRIV,1,"create_cookie() failed");
      exit(0);
   }
   set_cookie("cgipaf",cookie,"path=/");
   
   /* store the cookie and cookie timeout into the accessdb */
   
   if (save_access_status(accessdb,options[0][1],0,invalid_timeout,cookie)==-1) {
      printf_txt_msg("Warning: failed to update %s", accessdb);
      write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
      if (config_file != NULL) fclose(config_file);
      exit(0);

   }
   
   /* Gods become human */
   
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

   /* use run_viewmailcfg instead of built-in? */

   if((cp=get_sg_item(config_file,CFGSECTION,RUN_VIEWMAILCFG))!=NULL) {

      	xfree(cp);
      	i=run_cmd(config_file,CFGSECTION,RUN_VIEWMAILCFG,options);

      	if(i<0)
		write_log(LOG_USER,1,"run_viewmailcfg failed, run_cmd() returns %d",i);
      	else
		if((i=WEXITSTATUS(i)))
	  		write_log(LOG_USER,1,"run_viewmailcfg returns a non-null value %d",i);

      	if(i!=0) {
	 	snprintf(viewmailcfg_exitcode_txt,80,"%d",i);

	 	if(show_msg(config_file,doc_root,CFGSECTION,ERR_VIEWMAILCFGSCRIPT,err_viewmailcfgscript,options,txt_message)==2) printf("%d",i);
	 
		if(config_file!=NULL) fclose(config_file);

	 	exit(0);
      	}
   }
   else {


   	/* read the current user's mailcfg */
   
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
   	else { 
		write_log(LOG_USER,7,"autoreply is not enabled");
	}

   	autoreply_msg=get_vacations(pw);

   	if (autoreply_msg!=NULL) 
		options[13][1]=autoreply_msg;

   	if (strcmp(forward,"no")) 
		strcpy(not_forward,"no");

   	if (strcmp(keep_msg,"no")) 
		strcpy(not_keep_msg,"no");

   	if (strcmp(autoreply,"no")) 
		strcpy(not_autoreply,"no");

   	write_log(LOG_USER,7,"got the mailconfiguration for user %s",name);

   	if(show_msg(config_file,doc_root,CFGSECTION,CFG_MAILCFG,NULL,options,txt_message)==3) {

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
      		printf("<FORM ACTION=\"/cgi-bin/mailcfg.cgi\" METHOD=\"POST\">\n");
      		printf("<TABLE WIDTH=100%>\n");
      		printf("<TR>\n");
      		printf("<TD>Login name: %s</TD>\n",options[0][1]);
      		printf("<TD ALIGN=\"LEFT\"><INPUT NAME=name VALUE=\"%s\" TYPE=HIDDEN SIZE=10 MAXLENGTH=15></TD>\n",options[0][1]);
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
      		printf("<TEXTAREA NAME=\"autoreply_msg\" ROWS=\"10\" COLS=\"60\">");
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

   }

   	write_log(LOG_AUTHPRIV,6,"User %s has login successfully",name);
#endif
   if (config_file != NULL) fclose(config_file);
   exit(0);
}
