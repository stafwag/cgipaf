unsetenv("IFS");
set_memerr(out_of_memory);      /* set out of memory handler */
fflush(0);

/* reset parms */

memset(forward,'\0',sizeof(forward)); 
forward_to=txt_NULL;                  
memset(keep_msg,'\0',sizeof(keep_msg));
memset(autoreply,'\0',sizeof(autoreply));


/* init options table */

options=add_2_string_pair(options,txt_name,txt_NULL);						/* 0 */
options=add_2_string_pair(options,txt_max_length,max_length_txt);				/* 1 */
options=add_2_string_pair(options,txt_min_length,min_length_txt);              			/* 2 */
options=add_2_string_pair(options,txt_max_invalid,max_invalid_txt);            			/* 3 */
options=add_2_string_pair(options,txt_invalid_timeout,max_invalid_txt);        			/* 4 */
options=add_2_string_pair(options,txt_invalid_wait,invalid_wait_txt);          			/* 5 */
options=add_2_string_pair(options,txt_forward_to,txt_NULL);                    			/* 6 */
options=add_2_string_pair(options,txt_forward,forward);                        			/* 7 */
options=add_2_string_pair(options,txt_not_forward,not_forward);                			/* 8 */
options=add_2_string_pair(options,txt_keep_msg,keep_msg);                      			/* 9 */
options=add_2_string_pair(options,txt_not_keep_msg,not_keep_msg);              			/* 10 */
options=add_2_string_pair(options,txt_autoreply,autoreply);                    			/* 11 */
options=add_2_string_pair(options,txt_not_autoreply,not_autoreply);            			/* 12 */
options=add_2_string_pair(options,txt_autoreply_msg,txt_NULL);                 			/* 13 */
options=add_2_string_pair(options,txt_cookietimeout,cookie_timeout_txt);       			/* 14 */
options=add_2_string_pair(options,txt_password,txt_NULL);                      			/* 15 */
options=add_2_string_pair(options,txt_crackliberror,txt_NULL);					/* 16 */
options=add_2_string_pair(options,txt_pamerrormsg,txt_NULL);		       			/* 17 */
options=add_2_string_pair(options,txt_mailcfg_exitcode,mailcfg_exitcode_txt);  			/* 18 */
options=add_2_string_pair(options,txt_homedir,txt_NULL);                       			/* 19 */
options=add_2_string_pair(options,txt_domain,txt_NULL);                        			/* 20 */
options=add_2_string_pair(options,txt_badpassword,txt_NULL);		       			/* 21 */
options=add_2_string_pair(options,txt_usermaildomain,txt_NULL);	       				/* 22 */
options=add_2_string_pair(options,txt_message,txt_NULL);	       				/* 23 */
options=add_2_string_pair(options,txt_viewmailcfg_exitcode,viewmailcfg_exitcode_txt);          	/* 24 */

/*
 * test setuid(0)
 */

if (setuid(0)==-1) {
   write_log(LOG_USER,7,"setuid(0) failed");
   print_txt_msg(err_setuidzero);
   exit(0);
}

   /*
    * set doc_root & accessdb
    */
   
   /* Where are my documents??? */
   
   if ((config_file=fopen(CONFIGFILE,"r"))!=NULL) {
      doc_root=get_sg_item(config_file,CFGSECTION,DOC_ROOT);

      /* is logging enabled? */

      if ((cp=get_sg_item(config_file,CFGSECTION,CFG_SYSLOG))!=NULL) {
	 if (is_var_yes(cp)==0) enable_log(0);
	 xfree(cp);
      }

      /* set loglevel */

      if ((cp=get_config_item(config_file,CFG_LOGLEVEL))!=NULL) { 
	 sscanf(cp,"%d",&brol);
	 set_loglevel(brol);
	 xfree(cp);
	 write_log(LOG_USER,7,"Set loglevel to %d",set_loglevel(-1));
      }

      /* set_SCRIPT_FILENAME */
      
      if ((cp=get_sg_item(config_file,CFGSECTION,CFG_UNSET_SCRIPT_FILENAME))!=NULL) {
	 if (is_var_yes(cp)==0) set_script_filename=0;
	    else set_script_filename=2;
	 xfree(cp);
      }

      if ((cp=get_sg_item(config_file,CFGSECTION,CFG_SET_SCRIPT_FILENAME))!=NULL) {
	 if (is_var_yes(cp)==0) set_script_filename=0;
	    else set_script_filename=1;
	 xfree(cp);
      }
   }


#if defined(CGIPAF_MAILCFG) || defined(CGIPAF_VIEWMAILCFG)
  else {
      
      /* couldn't open the cfgfile, print errormessage and exit */
       
      print_txt_msg(err_mcfg_configfile);
      puts(CONFIGFILE);
      puts(txt_contact_webmaster);
      write_log(LOG_USER,3,"%s %s",err_mcfg_configfile,CONFIGFILE);
      exit(0);
   }
#endif

   if (config_file!=NULL) 
     write_log(LOG_USER,7,"configfile %s opened",CONFIGFILE);
   else {
      write_log(LOG_USER,1,"can't open configfile %s, %s",CONFIGFILE,strerror(errno));
      set_loglevel(7);
      write_log(LOG_USER,7,"failed to open configfile, switch to debug mode");
   }
   
   if (doc_root!=NULL) {
      write_log(LOG_USER,7,"doc_root set to %s",doc_root);
      if (chdir(doc_root)==-1) 
	write_log(LOG_USER,3,"chdir(%s) failed, %s",doc_root,strerror(errno));
   }

#ifdef _WITHPAM
   
   /* set the pam service name */
   
   if ((pam_servicename=get_sg_item(config_file,CFGSECTION,CFG_PAM_SERVICE))!=NULL) {
      set_pam_service(pam_servicename);
   }
   pam_servicename=set_pam_service(NULL);
   write_log(LOG_USER,7,"pam service name set to %s",pam_servicename);

#ifdef CGIPAF_PASSWD

   /* set the pam_chauth_flag */
   
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_PAM_CHANGE_EXPIRED_AUTHTOK))!=NULL) {
      if (is_var_yes(cp)!=1) {
	 write_log(LOG_USER,7,"set PAM_CHANGE_EXPIRED_AUTHOK to off");
	 set_pam_chauth_flag(0);
      }
      else {   
	 write_log(LOG_USER,7,"set PAM_CHANGE_EXPIRED_AUTHOK to on");
      }
      xfree(cp);
   }

#endif /* CGIPAF_PASSWD */
   
#endif /* _WITH_PAM */

#ifdef CGIPAF_PASSWD
#ifdef HAVE_LIBCRACK
   
   /* enable cracklib if cracklib is set to "on" */
   
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_CRACKLIB))!=NULL) {
      if (is_var_yes(cp)==0) {
	 enable_cracklib=0;
         write_log(LOG_USER,7,"cracklib disabled");
      }
      else {
	 enable_cracklib=1;
	 write_log(LOG_USER,7,"cracklib enabled");
      }
      xfree(cp);
}
#endif  /* HAVE_LIBCRACK */
#endif  /* CGIPAF_PASSWD */

   /* Should we use an accessdb, if yes get max_invalid & invalid_timeout */

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

#if defined(CGIPAF_MAILCFG) || defined(CGIPAF_VIEWMAILCFG)

   /* we need accessdb to store our cookies */
   
   if(accessdb==NULL) {
     write_log(LOG_USER,3,"%s",err_mcfg_accessdb);
     show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESSDB,err_mcfg_accessdb,options,txt_message);
     exit(0);
   }

   /*  get the cookie time out */
   
   cp=get_section_config_item(config_file,CFGSECTION,CFG_COOKIETIMEOUT); 

   if(cp!=NULL) sscanf(cp,"%d",&cookie_timeout);

   write_log(LOG_USER,7,"cookie_timeout set to %d",cookie_timeout);
   
   snprintf(cookie_timeout_txt,80,"%d",cookie_timeout);


#endif

#ifdef CGIPAF_MAILCFG

   /* set the sendmail cmd path */

   
   if ((sendmail=get_section_config_item(config_file,CFGSECTION,SENDMAIL))==NULL)
     sendmail=txt_sendmail;
   
   write_log(LOG_USER,7,"sendmail set to %s",sendmail);

   /* set the mailerdomain */ 
   
   domain=get_section_config_item(config_file,CFGSECTION,CFGDOMAIN);
     if(domain!=NULL) 
       write_log(LOG_USER,7,"domain set to %s",domain);
     else write_log(LOG_USER,7,"domain not set");
       domain=options[20][1]=get_maildomain(domain);

#endif

#ifdef CGIPAF_VIEWMAILCFG

   /* use a statefile or not */
   
   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_USE_STATEFILE))!=NULL) {
      if(is_var_yes(cp)!=0);
	use_mailcfg_statefile(1);
      write_log(LOG_USER,7,"use_state_file set to %d",use_mailcfg_statefile(-1));
      xfree(cp);
   }
   
   if ((cp=get_sg_item(config_file,CFGSECTION,RUN_MAILCFG))!=NULL) {
      use_mailcfg_statefile(1);
      write_log(LOG_USER,7,"run_mailcfg enabled, enable use_state_file");
      xfree(cp);
   }

#endif

#ifdef CGIPAF_PASSWD
   /* 
    * Get the max and min password length and store them in  
    * min_length / max_length
    */
   if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MINLENGTH))==NULL) 
     min_length=MINLENGTH;
   else { sscanf(cp,"%d",&min_length); }
   write_log(LOG_USER,7,"min_length set to %d",min_length);
   snprintf(min_length_txt,80,"%d",min_length);

   if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MAXLENGTH))==NULL) 
     max_length=MAXLENGTH;
   else { sscanf(cp,"%d",&max_length); }
   write_log(LOG_USER,7,"min_length set to %d",max_length);
   snprintf(max_length_txt,80,"%d",max_length);

   /*
    * Get the words not allowed in newpassword
    * Added by DJR
    */
    illegal_words=get_config_array(config_file,CFG_ILLEGALWORDS); /* will check for success of operation later */
   
   /* We dont want too much data */
	   
    max_postlength=MAXPOSTLENGTH;

   if ((cp=get_section_config_item(config_file,CFGSECTION,CFG_MAXPOSTLENGTH))==NULL) {
	   max_postlength=MAXPOSTLENGTH;
   }
	else {
		sscanf(cp,"%d",&max_postlength);
	}
   write_log(LOG_USER,7,"max_postlength set to %d",max_postlength);

   if ((cp=getenv("CONTENT_LENGTH"))!=NULL) {
      sscanf(cp,"%d",&brol);
      if (brol>max_postlength) {
	 print_html_msg(txt_too_much);
	 write_log(LOG_USER,3,"%s",txt_too_much);
	 exit(1);
      }
   }
   
#endif

   /* read the POST data */
   
   write_log(LOG_USER,7,"Reading POST data...");
   data=read_post();

   /* No Post? -> Ask the user to login */
   
   if (!data) {
      write_log(LOG_USER,7,"no POST data, show login_document");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,LOGIN_DOCUMENT,err_readdata,options,txt_message);
   }

   /* add post variabeles to the options table */

   options=add_post_2_string_pair(data,options);



