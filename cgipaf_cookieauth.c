   /* get the user info, if the user doesn't exsist show error and exit */
   
   if (!(pw=get_pw(name))) {
      write_log(LOG_AUTHPRIV,1,"try to use mailcfg.cgi as %s which doesn't exist");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
   };

   
   options[19][1]=pw->p->pw_dir;
   
   /* get the cookies */
   
   cookies=get_cookies();

   /* i need cookies or i leave */
   
   if(cookies==NULL) {
      write_log(LOG_AUTHPRIV,1,"get_cookies() failed");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIE,err_cookie,options,txt_message);
   }
   else {
      
      /* i want my cookie or i quit */
      
      if((cookie=get_cookie(cookies,"cgipaf"))==NULL) {
	 write_log(LOG_AUTHPRIV,1,"get_cookie() failed");
	 show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);
      }
      else{
	 
	 /* i dont like old cookies */

	 if (!cmp_access_cookie(accessdb,options[0][1],cookie,cookie_timeout)) {
	    write_log(LOG_AUTHPRIV,6,"cookie timeout for user %s",options[0][1]);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIETIMEOUT,err_cookietimeout,options,txt_message);
	 }
      }
   }


