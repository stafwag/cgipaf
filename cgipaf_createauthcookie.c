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
   
