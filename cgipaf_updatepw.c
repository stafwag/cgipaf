/*
 * cgipaf_updatepw.c
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

   /* finally one use how can remmber his password? */
   
   if ((brol=chpw(pw,newpass1))==PASS_SUCCESS) {
      show_msgs(config_file,doc_root,CFGSECTION,msg_success,msg_changed,options,txt_message);
      fflush(stdout);
      write_log(LOG_AUTHPRIV,6,"Password for %s was changed",options[0][1]);
      options[15][1]=newpass1;

      /*
       * reset uid to run the run_success script
       */

      if(setuid(0)==-1) {
        write_log(LOG_USER,7,"setuid(0) failed: %s",strerror(errno));
      };

      i=run_cmd(config_file,UPDATESECTION,RUN_SUCCESS,options,set_script_filename);
      options[15][1]=txt_NULL;
      if(i<0) {
	 if(i==-1) 
	   write_log(LOG_AUTHPRIV,1,"Can't execute run_success %s run_cmd() failed",strerror(errno));
      }
      else
	if((i=WEXITSTATUS(i)))
	  write_log(LOG_AUTHPRIV,1,"run_success returns a non-null exit code %d",i); 
      if (accessdb) {
	 if (save_access_status(accessdb,options[0][1],0,invalid_timeout,NULL)==-1) {
	    printf("%s %s",warn_update_accessdb,accessdb);
	    write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
	 }
      }
   }
   else {
      
      /* use cant remember his pw, tell them... */
      
#ifdef _WITHPAM
      if(last_pam_msg()!=NULL) {
	 options[17][1]=last_pam_msg();
	 if(show_msg(config_file,doc_root,CFGSECTION,ERR_PAMERROR,NULL,options,txt_message)<3) {
	    write_log(LOG_AUTHPRIV,1,"pam err %s",options[17][1]);
	    if (config_file!=NULL) fclose(config_file);   
	    exit(0);
	 }
      }
      i=brol;
      brol=-15;
#endif
      print_html_msg("<CENTER><H1><B><I>");
      printf("<BR>Can't update password<BR>%s",err_chpw[abs(brol)-1]);
      if (errno) printf(", %s<BR>",strerror(errno));
#ifdef _WITHPAM
      if(last_pam_msg()!=NULL) printf(", %s<BR>",last_pam_msg());
      else printf(", %s<BR>",pam_strerror(pw->pamh,i));
#endif
      printf("</I></B></H1></CENTER>");
      write_log(LOG_AUTHPRIV,1,"Can't update password %s",err_chpw[abs(brol)-1]);
#ifdef _WITHPAM
      write_log(LOG_AUTHPRIV,1,"pam error %s",pam_strerror(pw->pamh,i));
#endif
   }

