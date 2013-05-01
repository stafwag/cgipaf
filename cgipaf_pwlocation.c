/*
 * cgipaf_pwlocation.c
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

#ifndef _WITHPAM
#ifdef  BSDHOST
     set_shadow_location(NULL);
     set_passwd_location(NULL);
#else

    /*
     * vmail support
     * set name to name - @some.domain
     * set usermaildomain to @some.domain
     */

    if((cp=get_sg_item(config_file,CFGSECTION,CFG_VMAIL_SUPPORT))!=NULL) {
      if(is_var_yes(cp)) { 
	
	/* we don't like ".." in the loginname for security reasons */

	if(strstr(name,"..")) {
            write_log(LOG_USER,7,"vmail: loginname contains \"..\"");
            show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_ACCESS,err_access,options,txt_message);

	}
    	if((cp=strchr(name,'@'))!=NULL) {
      	  int l;
      	  usermaildomain=(char *) xmalloc(strlen(cp+1)+1);
	  options[22][1]=usermaildomain;
      	  strcpy(options[22][1],cp+1);
      	  l=cp-name;
      	  cp=name;
      	  name=xmalloc(l+1);
      	  strncpy(name,cp,l);
          name[l]='\0';
      	  write_log(LOG_USER,7,"vmail: loginname set to %s",name);
      	  write_log(LOG_USER,7,"vmail: options[0][1] set to %s",options[0][1]);
      	  write_log(LOG_USER,7,"vmail: usermaildomain set to %s",options[22][1]);
	};
      };
      xfree(cp);
    };


    /* 
    * passwordfile location
    */

   /* get passwd location */

   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_PASSWD_LOCATION))!=NULL) {
	   cp=add_parms(cp,options);
	   set_passwd_location(cp);
           write_log(LOG_USER,7,"passwd_location set to %s",set_passwd_location(NULL));
   }

   /* get shadow location */

   if ((cp=get_sg_item(config_file,CFGSECTION,CFG_SHADOW_LOCATION))!=NULL) {
	   cp=add_parms(cp,options);
	   set_shadow_location(cp);
           write_log(LOG_USER,7,"passwd_location set to %s",set_passwd_location(NULL));
   }

#endif
#endif
