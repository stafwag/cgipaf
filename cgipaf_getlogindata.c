/*
 * cgipaf_getlogindate.c
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


/* If there is no loginname ask for it */
   
   cp=get_postitem(data,LOGIN);

   if (!cp || !strlen(cp)) {
      write_log(LOG_USER,7,"%s not set, show err_login",LOGIN);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options,txt_message);
   }
   login=textarea2asc(cp);
   /* xfree(cp); */			/* keep the orginal post array */
   
   /* Make the login available to a redirect / ePHP */
   
   if (login!=NULL) {
      write_log(LOG_USER,7,"%s set to %s",LOGIN,login);
      cp=xmalloc(strlen(login)+1);
      strcpy(cp,login);
      update_string_pair_item(options,LOGIN,login,0);
   }


   /* We dont like too short loginnames */
   
   if (strlen(login)<1) {
      write_log(LOG_USER,7,"%s is invalid strlen(login)=0",login);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_LOGIN,err_loginname,options,txt_message);
   }

   /* root = god, god should know how to change his password */
   
   if (!strcmp(login,ROOT)) {
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

