/*
 * cgipaf_noroot.c
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


/* if login is NULL or has no lenght show error and exit */
   
   if (login==NULL) {
      write_log(LOG_AUTHPRIV,3,err_mcfg_readname);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options,txt_message);
   }
   if (strlen(login)<1) {
      write_log(LOG_AUTHPRIV,3,err_mcfg_readname);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_READNAME,err_mcfg_readname,options,txt_message);
   }

   /* if login="root" show error and exit */
   
   if (!strcmp(login,ROOT)) {
      write_log(LOG_AUTHPRIV,5,warn_root);
      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);
   }

