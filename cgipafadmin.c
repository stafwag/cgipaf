/*
 * cgipafadmin.c
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

#include "cgipaf_defs.h"

main()
{
	char **admins;
	int  is_admin=0;
	char *user=NULL;
	char *admin=NULL;

#include "cgipaf_vars.h"
#include "cgipaf_init.c"

	/*
	 * Read the admins out the config_file
	 */
	
	if((admins=get_section_config_array(config_file,CFGSECTION,CFG_ADMINUSERS))==NULL) {
             write_log(LOG_USER,7,"no admin users defined in config_file");
	     show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NOADMINS,err_noadmins,options,txt_message);
	}

	/*
	 * read post[user]
	 */


	if (!(cp=get_postitem(data,USER))) {

		/*
	 	 * If %{post[user] is not set display the login page
	 	 */

	#include "cgipaf_getlogindata.c"
	#include "cgipaf_noroot.c"

	#include "cgipaf_auth.c"

	}

	else {

   		user=cp;

		#include "cgipaf_setname2postname.c"
		#include "cgipaf_noroot.c"
		#include "cgipaf_cookieauth.c"

	}

	/*
	 * is name an admin???
	 */

	for(ccp=admins;*ccp!=NULL;ccp++) {

		if(!strcmp(name,*ccp)) {

			/*
			 * update options table and exit
			 */

			is_admin=1;
			admin=name;
			update_string_pair_item(options,txt_admin,admin,0);
			break;

		}


	}


if(!is_admin) {

	/* not an admin -> exit  */

      	write_log(LOG_AUTHPRIV,6,"%s is not an admin",name);
      	show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_INVALID,err_invalid,options,txt_message);

}

/*
 * If user is not defined assume we are at the first stage
 */


if(user==NULL) {

#include "cgipaf_createauthcookie.c"

   /*
    * display the admin page
    */

#ifdef CGIPAF_PWADMIN

	if(show_msg(config_file,doc_root,CFGSECTION,CFG_ADMIN_PASSWD,NULL,options,txt_message)==3) {

      /*
       * No admin page! use the default
       */

      print_html_msg("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
      puts("<html>");
      puts("<head>");
      puts("<title>Password Admin</title>");
      puts("</head");
      puts("<body>");
      printf("<h1>Welcome %s</h1>",options[25][1]);
      puts("<form action=\"/cgi-bin/pwadmin.cgi\" method=\"POST\">");
      printf("<input type=\"hidden\" name=\"name\" value=\"%s\">",options[25][1]);
      puts("<center>");
      puts("<table>");
      puts("<tr>");
      puts("<td>User</td>");
      puts("<td>:</td>");
      puts("<td>");
      puts("<input name=\"user\">");
      puts("</td>");
      puts("</tr>");
      puts("<tr>");
      puts("<td>New password</td>");
      puts("<td>:</td>");
      puts("<td><input name=newpass1 size=\"16\" type=password></td>");
      puts("</tr>");
      puts("<tr>");
      puts("<td>Re-enter new password</td>");
      puts("<td>:</td>");
      puts("<td><input name=newpass2 size=\"16\" type=password></td>");
      puts("</tr>");
      puts("</table>");
      puts("<p>");
      puts("<input type=\"reset\" value=\"Clear\">");
      puts("<input type=\"submit\" value=\"Update  Password\">");
      puts("</center>");
      puts("</form>");
      puts("</body>");
      puts("</html>");


   }

#endif
#ifdef CGIPAF_MAILADMIN


      if(pass!=NULL) {


	if(show_msg(config_file,doc_root,CFGSECTION,CFG_SELECT_USER,NULL,options,txt_message)==3) {

      /*
       * No admin page! use the default
       */

      
      print_html_msg("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
      puts("<html>");
      puts("<head>");
      puts("<title>Mailcfg Admin</title>");
      puts("</head");
      puts("<body>");
      printf("<h1>Welcome %s</h1>",options[25][1]);
      puts("<form action=\"/cgi-bin/mailadmin.cgi\" method=\"POST\">");
      printf("<input type=\"hidden\" name=\"name\" value=\"%s\">",options[25][1]);
      puts("<center>");
      puts("<table>");
      puts("<tr>");
      puts("<td>User</td>");
      puts("<td>:</td>");
      puts("<td>");
      puts("<input name=\"user\"");
      puts("</td>");
      puts("</tr>");
      puts("</table>");
      puts("<p>");
      puts("<input type=\"reset\" value=\"Clear\">");
      puts("<input type=\"submit\" value=\"Update Mail Configuratie\">");
      puts("</center>");
      puts("</form>");
      puts("</body>");
      puts("</html>");

	}

      }
      else {
	
	      pw=get_pw(name);

        
	      if(pw==NULL) {
		      write_log(LOG_USER,7,"get_pw(%s) failed, invalid user?",name,name);
		      show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NOSUCHUSER,err_nosuchuser,options,txt_message);
	}

	      #include "cgipaf_getmailcfgpost.c"
	      #include "cgipaf_updatemailcfg.c"

      }
	      

#endif

}

else {

	name=user;
	options[0][1]=name;
	write_log(LOG_USER,7,"admin setto %s",admin);
	write_log(LOG_USER,7,"user setto %s",user);

	pw=get_pw(name);

        if(pw==NULL) {
		write_log(LOG_USER,7,"get_pw(%s) failed, invalid user?",name,name);
      		show_msg_and_exit(config_file,doc_root,CFGSECTION,ERR_NOSUCHUSER,err_nosuchuser,options,txt_message);
	}

#ifdef CGIPAF_PWADMIN
	pass=NULL;
	

	#ifdef _WITHPAM
   
   	set_pam_chauth_flag(0);
   
	#endif

	write_log(LOG_USER,7,"starting cgipaf_checkpw...");


	#include "cgipaf_checkpw.c"
	write_log(LOG_USER,7,"starting cgipaf_updatepw...");
	#include "cgipaf_updatepw.c"

#endif
#ifdef CGIPAF_MAILADMIN

	#include "cgipaf_viewmailcfg.c"

#endif
}


if (config_file != NULL) fclose(config_file);
exit(0);

}
