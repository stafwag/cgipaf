   /* If there is no loginname ask for it */
   
   cp=get_postitem(data,LOGIN);

   if (!cp || !strlen(cp)) {
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

   cp=get_postitem(data,PASSWORD);

   if ( !cp || !strlen(cp) ) {
      write_log(LOG_USER,7,"user forgot to type his password");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
   }

   pass=textarea2asc(cp);
   
   /* free(cp); */			/* keep the orginal postarray */

