/*
 * cgipaf_cookieauth.c
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

/* get the user info, if the user doesn't exsist show error and exit */
   
   if (!(pw=get_pw(login))) {
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

	 if (cmp_access_cookie(accessdb,login,cookie,cookie_timeout)!=1) {
	    write_log(LOG_AUTHPRIV,6,"cookie timeout for user %s",login);
	    show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_COOKIETIMEOUT,err_cookietimeout,options,txt_message);
	 }
      }
   }


