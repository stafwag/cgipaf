/*
 * access control lists handeling 
 *
 * Copyright (C) 2002,2003 Staf Wagemakers Belgie/Belgium
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
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
