  /* 
    * access control lists handeling 
    */

   /* get UserAclOrder */

   if ((i=get_useraclorder(config_file,CFGSECTION))==-1) {
      write_log(LOG_USER,7,"get_useraclorder() failed, check configuration file");
      printf_txt_msg("%s, %s",err_cfg,err_useraclorder);
      exit(0);
   }
   else {
      write_log(LOG_USER,7,"get_useraclorder() returns %d",i);
   }

   /* Test User ACL */

   if(!user_acl(config_file,CFGSECTION,pw)) {
      write_log(LOG_AUTHPRIV,6,"user_acl(): user access denied");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   }


   /* get GroupAclOrder */

   if ((i=get_groupaclorder(config_file,CFGSECTION))==-1) {
      write_log(LOG_USER,7,"get_groupaclorder() failed, check configuration file");
      printf_txt_msg("%s, %s",err_cfg,err_groupaclorder);
      exit(0);
   }
   else {
      write_log(LOG_USER,7,"get_groupaclorder() returns %d",i);
   }
   
   /* Test Group ACL */

   if(!group_acl(config_file,CFGSECTION,pw)) {
      write_log(LOG_AUTHPRIV,6,"group_acl(): group access denied");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options);
   }
 
