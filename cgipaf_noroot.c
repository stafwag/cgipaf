   /* if name is NULL or has no lenght show error and exit */
   
   if (name==NULL) {
      write_log(LOG_AUTHPRIV,3,err_mcfg_readname);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options,txt_message);
   }
   if (strlen(name)<1) {
      write_log(LOG_AUTHPRIV,3,err_mcfg_readname);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options,txt_message);
   }

   /* if name="root" show error and exit */
   
   if (!strcmp(name,ROOT)) {
      write_log(LOG_AUTHPRIV,5,warn_root);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);
   }

