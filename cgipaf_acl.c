  /* 
   * access control lists handeling 
   */

   /* get AclOrder */

   if ((i=get_aclorder(config_file,CFGSECTION))==-1) {
      write_log(LOG_USER,7,"get_aclorder() failed, check configuration file");
      printf_txt_msg("%s, %s",err_cfg,err_aclorder);
      exit(0);
   }

   /* Test ACL */

   if(!acl(config_file,CFGSECTION,pw,usermaildomain)) {
      write_log(LOG_AUTHPRIV,6,"acl(): access denied");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
   }
