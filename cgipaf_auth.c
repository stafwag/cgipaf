/*
 * cgipaf_auth.c
 *
 * Copyright (C) 2004 Staf Wagemakers Belgie/Belgium
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


    #include "cgipaf_pwlocation.c"

   /*
    * test pass, and create accessdb
    */
   
   /* fetch the user data, if the user dont exist... dont tell them */
   
   if (!(pw=get_pw(login))) {
      write_log(LOG_USER,7,"get_pw(%s) failed, user %s doesn't exists",login,login);
      write_log(LOG_AUTHPRIV,6,"Invalid password for user %s",login);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);
   };
   write_log(LOG_USER,7,"pw->p->pwuid = %d",pw->p->pw_uid);

#ifdef _WITHPAM
  
   /*
    * Set the real uid to the user's uid, set the effective uid to root
    */
   
   if(setreuid(pw->p->pw_uid,0)==-1) {
        write_log(LOG_USER,7,"setreuid() failed: %s",strerror(errno));
   }; 

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
	 i=run_cmd(config_file,CFGSECTION,RUN_LOCKED,options,set_script_filename);
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
      write_log(LOG_AUTHPRIV,6,"Invalid password for user %s",login);
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
   else {
	   write_log(LOG_USER,7,"ckpw() succeed");
   }
   options[19][1]=pw->p->pw_dir;
	   write_log(LOG_USER,7,"DEBUG: cgipaf_auth ends");


