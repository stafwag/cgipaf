/*
 * cgipaf_vars.h
 *
 * Copyright (C) 1999,2000-04 Staf Wagemakers Belgie/Belgium
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

#ifndef _CGIPAF_VARS_H
#define _CGIPAF_VARS_H

int    i,brol;
int    max_invalid=3;
int    invalid_timeout=600;
int    enable_cracklib=0;
struct pw_info *pw;
WEBDATA *data,*get_data;
char   *login=NULL,*pass=NULL,*usermaildomain=NULL;
char   *cracklib_dictpath=NULL;
FILE   *config_file=NULL;
char   *doc_root=NULL;
char   txt_name[]="name";
char   txt_NULL[]="\0";
char   txt_max_invalid[]="max_invalid";
char   txt_invalid_timeout[]="invalid_timeout";
char   txt_invalid_wait[]="invalid_wait";
char   txt_too_much[]="<CENTER><H1><B><I><BR>Too much data!<BR></I></B></H1></CENTER>";
char   *cookie=NULL;
time_t cookie_timeout=300;
char   max_invalid_txt[80]="3";
char   invalid_timeout_txt[80]="600";
char   invalid_wait_txt[80]="0";
char   *accessdb=NULL; 
char   *cp=NULL;
char   **ccp=NULL;
char   *newpass1=NULL,*newpass2=NULL;
char   txt_max_length[]="max_length";
char   **illegal_words; 		/* Added by DJR */
char   txt_min_length[]="min_length";
char   max_length_txt[80]="8";
char   min_length_txt[80]="6";
char   txt_forward[]="forward";
char   forward[10]="no";
char   txt_not_forward[]="not_forward";
char   not_forward[10]="yes";
char   txt_forward_to[]="forward_to";
char   *forward_to=NULL;
char   txt_keep_msg[]="keep_msg";
char   keep_msg[10]="no";
char   txt_not_keep_msg[]="not_keep_msg";
char   not_keep_msg[10]="yes";
char   txt_autoreply[]="autoreply";
char   autoreply[10]="no";
char   txt_not_autoreply[]="not_autoreply";
char   not_autoreply[10]="yes";
char   txt_autoreply_msg[]="autoreply_msg";
char   txt_cookietimeout[]="cookie_timeout";
char   cookie_timeout_txt[80]="300";
char   *autoreply_msg=txt_NULL;
char   txt_yes[]="yes";
char   txt_no[]="no";
char   txt_password[]="password";
char   txt_passwd[]="passwd";
char   txt_crackliberror[]="cracklib_error";
char   txt_mailcfg_exitcode[]="mailcfg_exitcode";
char   txt_viewmailcfg_exitcode[]="viewmailcfg_exitcode";
char   txt_homedir[]="homedir";
char   txt_domain[]="domain";
char   mailcfg_exitcode_txt[80]="\0";
char   viewmailcfg_exitcode_txt[80]="\0";
char   txt_pamerrormsg[]="pam_error";
char   *pam_servicename=txt_passwd;
char   txt_badpassword[]="bad_password";
int    max_length, min_length;
char   txt_usermaildomain[]="user_maildomain";
char   txt_message[]="message";
char   txt_admin[]="admin";
char   txt_user[]="user";
char   txt_cgipath[]="cgi_path";
char   *msg_success[]={"msg_success","msg_updated","msg_changed",NULL};
char   ***options=NULL;
int    max_postlength=150;
int    set_script_filename=0;
char   *cgi_path=NULL;
char   *next_actionfull=NULL;
char   *login_actionfull=NULL;
char   txt_nextaction[]="next_action";
char   txt_nextactionfull[]="next_action_full";

WEBDATA *cookies=NULL;

#if defined(CGIPAF_MAILCFG) || defined(CGIPAF_MAILADMIN)

   int    forward_state=0;
   int    keep_state=0;
   int    autoreply_state=0;
   int    oldstate=0;
   int    newstate=0;
   int 	  mailcfg_check=1;
   char   *c=NULL;
   char   *sendmail=NULL;
   char   *domain=NULL;
   char   txt_sendmail[]="/usr/lib/sendmail";
   FILE   *fp;

#endif



#endif  /* _CGIPAF_VARS_H */
	 
