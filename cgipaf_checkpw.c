/* get the new password or die */
   
   cp=get_postitem(data,NEWPASS1);

   if (!cp || !strlen(cp)) {
      write_log(LOG_USER,7,"newpass1 not set");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NEWPASS,err_newpass,options,txt_message);
   }
   newpass1=textarea2asc(cp);

   /* free(cp); */			/* keep the orginal postarray */

   cp=get_postitem(data,NEWPASS2);

   if (!cp || !strlen(cp)) {
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

   if(pass!=NULL) {
   
   if (strcmp(newpass1,pass)==0) {
      write_log(LOG_USER,7,"password unchanged");
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_UNCHANGED,err_unchanged,options,txt_message);
   }

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
      write_log(LOG_USER,7,"new password too long, max_length=%d got %d",max_length,strlen(newpass2));
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


