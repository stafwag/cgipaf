/*
 * cgipaf_setname2postname.c
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
   

/* set name to the loginname send by viewmailcfg.cgi */
   
   login=NULL;
   if ((cp=get_postitem(data,LOGIN))!=NULL) {
      login=textarea2asc(cp);
      xfree(cp);
      cp=xmalloc(strlen(login)+1);
      strcpy(cp,login);
      update_string_pair_item(options,LOGIN,login,0);
   }

