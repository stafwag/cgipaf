   /* Is mailcfg check enabled? */

   if((cp=get_section_config_item(config_file,UPDATESECTION,RUN_MAILCFG))==NULL) {

      /* if there is no run_mailcfg script, mailcfg_check is always enabled */

      xfree(cp);
      mailcfg_check=1;
      write_log(LOG_USER,7,"no run_mailcfg script -> mailcfg check enabled");

   }

   else {

	 /* get the mailcfg_check setting out the cfgfile */
   
         if ((cp=get_section_config_item(config_file,UPDATESECTION,CFG_MAILCFG_CHECK))!=NULL) {

            if (is_var_yes(cp)==0) {

	       mailcfg_check=0;
               write_log(LOG_USER,7,"mailcfg check disabled");

            }
            else {
	           mailcfg_check=1;
	           write_log(LOG_USER,7,"mailcfg check enabled");
            }

         }
         else {
		 /* no mailcfg_check setting, mailcfg_check is enabled by default */
		 mailcfg_check=1;
	         write_log(LOG_USER,7,"mailcfg_check is not defined, using default (enabled)");


	 }

    }

   
   if(mailcfg_check) {


      /* if forward has no length show error and exit */
   
      if (strlen(forward)<1) {

     	    write_log(LOG_USER,5,err_forward);
      	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_FORWARD,err_forward,options,
			      txt_message);
      }

      /* if mailforwarding is enabled evaluate forward_to and keep_msg */
   
      if (!strcasecmp(forward,txt_yes)) {

     	 write_log(LOG_USER,7,"User %s want to enable mailforwarding",name);
      
      	 /* forward_to has no length or is an invalid mailaddress, exit */
      
      	 if (strlen(forward_to)<1) {

	    write_log(LOG_USER,7,"forward_to has zero length, display err_forwardto");
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_FORWARDTO,err_forwardto,options,
			      txt_message);
         }

         if (!tst_emailaddress(forward_to)) {
	 
	    write_log(LOG_USER,7,"forward_to has zero length, display err_forwardto");
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALIDFORWARDTO,
			      err_invalidforwardto,options,txt_message);
         } 

         /* if keep_msg has no length exit */
      
         if (strlen(keep_msg)<1) {

	    write_log(LOG_USER,5,err_keep_msg);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_KEEP_MSG,err_keep_msg,options,
			      txt_message);
         }
     }

     /* if autoreply has no length show error and exit */
   
     if (strlen(autoreply)<1) {

        write_log(LOG_USER,5,err_autoreply);
        show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_AUTOREPLY,err_autoreply,options,
		  	  txt_message);
     }
   
     /* if autoreply is enabled check autoreply_msg */
   
     if ((strlen(autoreply_msg)<1)&&!strcasecmp(autoreply,txt_yes)) {

        write_log(LOG_USER,7,"autoreply is enabled, and autoreply_msg has zero length, display err_autoreplymsg");
        show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_AUTOREPLYMSG,err_autoreplymsg,options,
			  txt_message);

     }

   }


#include "cgipaf_pwlocation.c"

   if (usermaildomain!=NULL) {
      domain=options[20][1]=usermaildomain;
   }


#include "cgipaf_cookieauth.c"

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


   if (mailcfg_check) {

      /* get the previous mailcfg state out the user's .cgipaf_state file */
      
      if ((oldstate=get_mailcfg_status(pw))==-1) 
         write_log(LOG_USER,7,
		 "failed to read old status file .cgipaf_state: %s for user %s",strerror(errno));
   
      /* calculate the new state */
   
      newstate=forward_state+2*keep_state+4*autoreply_state; 
         
         write_log(LOG_USER,7,"newstate = %d",newstate);
         write_log(LOG_USER,7,"oldstate = %d",newstate);
   
      if (oldstate==-1) oldstate=0;  /* if .cgipaf_state doesnt exists asume state 0 */
   
      /* if not configured  -> configured
       * run run_before_mailcfg
       */
   
      if (!oldstate&&newstate) {

         i=run_cmd(config_file,UPDATESECTION,RUN_BEFORE_MAILCFG,options,set_script_filename);

         if (i<0) {

	    if (i==-1)
	       write_log(LOG_USER,1,"Can't executed %s %s",RUN_BEFORE_MAILCFG,strerror(errno));
         }
         else
	      if ((i=WEXITSTATUS(i)))
	         write_log(LOG_USER,1,"run_before returns a non-null exitcode %d",i);
      }
   
      /* copy the user's message to $HOME/vacations.txt */
      
      if (!(fp=fopen(add2home(pw->p,"vacations.txt"),"w"))) {

         write_log(LOG_USER,1,err_openvacations);	
         show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_OPENVACATIONS,err_openvacations,
			   options,txt_message);

      }
      fputs(autoreply_msg,fp);
      fclose(fp);

   }

   /* use run_mailcfg instead of built-in? */
   
   if ((cp=get_section_config_item(config_file,UPDATESECTION,RUN_MAILCFG))!=NULL) {

      xfree(cp);
      write_log(LOG_USER,7,"starting run_mailcfg");
      i=run_cmd(config_file,UPDATESECTION,RUN_MAILCFG,options,set_script_filename);
      if(i<0)
	write_log(LOG_USER,1,"run_mailcfg failed, run_cmd() returns %d",i);
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_USER,1,"run_mailcfg returns a non-null value %d",i);
      if(i!=0) {
	 snprintf(mailcfg_exitcode_txt,80,"%d",i);
	 if(show_msg(config_file,doc_root,CFGSECTION,ERR_MAILCFGSCRIPT,err_mailcfgscript,options,txt_message)==2) printf("%d",i);
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
	       xfree(c);
	       write_log(LOG_USER,1,err_delforward);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_DELFORWARD,err_delforward,options,txt_message);
	    } 
	 }
	 xfree(c);
      }
 
      /* deletes the user's .procmail */
   
      c=add2home(pw->p,PROCMAIL);
      if (c!=NULL) {
	 if (unlink(c)==-1) {
	    if (!(errno==ENOENT)) {
	       xfree(c);
	       write_log(LOG_USER,1,err_delprocmail);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_DELPROCMAIL,err_delprocmail,options,txt_message);
	    } 
	 }
	 xfree(c);
      }
      
      
      /* if auto replay if enabled create a new .procmailrc */
      
      if(!strcmp(autoreply,txt_yes)||!strcmp(forward,txt_yes)) {
	 if(write_procmailrchead(pw,sendmail)==-1) {
	    write_log(LOG_USER,1,"%s write_procmailrchead() failed",err_updateprocmailrc);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options,txt_message);
	 }
      }
      
      if (!strcmp(autoreply,txt_yes)) {
	 if(enable_reply(pw,domain)==-1) {
	    write_log(LOG_USER,1,"%s enable_reply() failed",err_updateprocmailrc);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options,txt_message);
	 }
      }
      
      /* enable mail forwardin in .procmailrc */
      
      if (!strcmp(forward,txt_yes)) {
	 if (!strcmp(keep_msg,txt_yes)) {
	    if (enable_kforward(pw,textarea2asc(forward_to),domain)==-1) {
	       write_log(LOG_USER,1,"%s enable_kforward() failed",err_updateprocmailrc);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options,txt_message);
	    }
	 }
	 else {
	    if(enable_forward(pw,textarea2asc(forward_to),domain)==-1) {
	       write_log(LOG_USER,1,"%s enable_forward() failed",err_updateprocmailrc);
	       show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UPDATEPROCMAILRC,err_updateprocmailrc,options,txt_message);
	    }
	 }
      }
   }

   if (mailcfg_check) {
   
      /* mail config update, inform the user */
      
      show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_updated,options,txt_message);
      write_log(LOG_AUTHPRIV,6,"User %s has updated his mail configuration successfully",name);
      
      /* create the user's .cgipaf_state */
      
      if (save_mailcfg_status(pw->p,forward_state,textarea2asc(forward_to),
			     keep_state,autoreply_state)==-1) {

         write_log(LOG_USER,1,"Can't create statefile %s",strerror(errno));
   
      }
   
      /* start run_success if defined */
   
      i=run_cmd(config_file,UPDATESECTION,RUN_SUCCESS,options,set_script_filename);
      if (i<0) {
         if (i==-1)
	    write_log(LOG_USER,1,"Can't executed run_success %s",strerror(errno));
      }
      else
         if ((i=WEXITSTATUS(i)))
            write_log(LOG_USER,1,"run_success returns a non-null value",i);

      /* 
       * if configured -> not configured
       * run run_after_mailcfg
       */
   
      if (oldstate&&!newstate) {

	 i=run_cmd(config_file,UPDATESECTION,RUN_AFTER_MAILCFG,options,set_script_filename);

      if(i<0) {
	 if(i==-1)
	   write_log(LOG_USER,1,"Can't executed %s %s",RUN_BEFORE_MAILCFG,strerror(errno));
      }
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_USER,1,"run_after_mailcfg returns a non-null value %d",i);
      }

   }
