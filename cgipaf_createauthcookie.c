/*
 * cgipaf_createauthcookie.c
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


/* make a cookie and give it to the use keep them sweet... */
   
   cookie=create_cookie();
   if(cookie==NULL) {
      print_txt_msg("Can't create cookie");
      write_log(LOG_AUTHPRIV,1,"create_cookie() failed");
      exit(0);
   }
   set_cookie("cgipaf",cookie,"path=/");
   
   /* store the cookie and cookie timeout into the accessdb */
   
   if (save_access_status(accessdb,login,0,invalid_timeout,cookie)==-1) {
      printf_txt_msg("Warning: failed to update %s", accessdb);
      write_log(LOG_AUTHPRIV,1,"%s %s",warn_update_accessdb,accessdb);
      if (config_file != NULL) fclose(config_file);
      exit(0);

   }
   
