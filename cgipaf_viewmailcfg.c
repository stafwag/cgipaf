   
#include "cgipaf_createauthcookie.c"

   /* Gods become human */
   
   write_log(LOG_USER,7,"set uid to %d",pw->p->pw_uid);
   if (setgid(pw->p->pw_gid)==-1) {
      write_log(LOG_USER,7,"setgid() failed");
   };
   write_log(LOG_USER,7,"set gid to %d",pw->p->pw_gid);
   if (setuid(pw->p->pw_uid)==-1) {
      write_log(LOG_USER,7,"setuid() failed");
   }
   write_log(LOG_USER,7,"set umask to 0177");
   umask(0177);

   /* use run_viewmailcfg instead of built-in? */

   if((cp=get_sg_item(config_file,UPDATESECTION,RUN_VIEWMAILCFG))!=NULL) {

      	xfree(cp);
      	i=run_cmd(config_file,UPDATESECTION,RUN_VIEWMAILCFG,options,set_script_filename);

      	if(i<0)
		write_log(LOG_USER,1,"run_viewmailcfg failed, run_cmd() returns %d",i);
      	else
		if((i=WEXITSTATUS(i)))
	  		write_log(LOG_USER,1,"run_viewmailcfg returns a non-null value %d",i);

      	if(i!=0) {
	 	snprintf(viewmailcfg_exitcode_txt,80,"%d",i);

	 	if(show_msg(config_file,doc_root,CFGSECTION,ERR_VIEWMAILCFGSCRIPT,err_viewmailcfgscript,options,txt_message)==2) printf("%d",i);
	 
		if(config_file!=NULL) fclose(config_file);

	 	exit(0);
      	}
   }
   else {


   	/* read the current user's mailcfg */
   
   	write_log(LOG_USER,7,"reading current mail configuration....");

   	if((forward_to=get_kforward(pw))!=NULL) {

      		strcpy(forward,"yes"); strcpy(keep_msg,"yes");
      		write_log(LOG_USER,7,"forwarding with keepmsg is enabled in current cfg, mail are forwarded to %s",forward_to);
   	}
   	else {

      		if ((forward_to=get_forward(pw))!=NULL) {
	 		strcpy(forward,"yes");
			strcpy(keep_msg,"no");
	 		write_log(LOG_USER,7,"forwarding is enabled in current cfg, mail are forwarded to %s",forward_to);
      		}
      		else {
			strcpy(forward,"no");
			strcpy(keep_msg,"no");
	 		forward_to=txt_NULL;
	 		write_log(LOG_USER,7,"mailforwarding is not enabeled");
      		}
   	};

   	options[6][1]=forward_to;

   	if (get_reply(pw)) {

      		strcpy(autoreply,"yes");
      		write_log(LOG_USER,7,"autoreply is enabled in the current cfg");
   
	}
   	else {
		strcpy(autoreply,"no");
		write_log(LOG_USER,7,"autoreply is not enabled");
	}

   	autoreply_msg=get_vacations(pw);

   	if (autoreply_msg!=NULL) 
		options[13][1]=autoreply_msg;

   	if (strcmp(forward,"no")) 
		strcpy(not_forward,"no");
	else 
		strcpy(not_forward,"yes");

   	if (strcmp(keep_msg,"no")) 
		strcpy(not_keep_msg,"no");
	else 
		strcpy(not_keep_msg,"yes");

   	if (strcmp(autoreply,"no")) 
		strcpy(not_autoreply,"no");
 	else
		strcpy(not_autoreply,"yes");

   	write_log(LOG_USER,7,"got the mailconfiguration for user %s",name);

   	if(show_msg(config_file,doc_root,CFGSECTION,CFG_MAILCFG,NULL,options,txt_message)==3) {

      		write_log(LOG_USER,7,"couldn't load mailcfg_document using default");
      		print_html_msg("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
      		printf("<HTML>\n");
      		printf("<HEAD>\n");
      		printf("<TITLE>Change your mailbox configuration</TITLE>");
      		printf("</HEAD>\n");
      		printf("<BODY text=\"#000000\" bgcolor=\"#B0B0B0\">\n");
      		printf("<H1>Change your mailbox configuration</H1>\n");
      		printf("<HR>\n");
      		printf("<P>\n");
      		printf("<FORM ACTION=\"/cgi-bin/mailcfg.cgi\" METHOD=\"POST\">\n");
      		printf("<TABLE WIDTH=100%>\n");
      		printf("<TR>\n");
      		printf("<TD>Login name: %s</TD>\n",options[0][1]);
      		printf("<TD ALIGN=\"LEFT\"><INPUT NAME=name VALUE=\"%s\" TYPE=HIDDEN SIZE=10 MAXLENGTH=15></TD>\n",options[0][1]);
      		printf("</TR>\n");
      		printf("</TABLE>\n");
      		printf("<HR>\n");
      		printf("<TABLE>\n");
      		printf("<TR>\n");
      		printf("<TD>Forward mailbox:</TD>\n");
      		printf("<TD WIDTH=20>\n");
      		printf("<SELECT NAME=\"forward\">\n");
      		printf("<OPTION value=\"%s\">%s\n",forward,forward);
      		printf("<OPTION value=\"%s\">%s\n",not_forward,not_forward);
      		printf("</SELECT></TD>\n");
      		printf("<TD>Forward to:</TD>\n");
      		printf("<TD><INPUT NAME=forward_to VALUE=\"%s\" TYPE=TEXT SIZE=30 MAXLENGTH=40></TD>\n",forward_to);
      		printf("</TR>\n");
      		printf("<TR>\n");
      		printf("<TD>Keep messages:</TD>\n");
      		printf("<TD>\n");
      		printf("<SELECT NAME=\"keep_msg\">\n");
      		printf("<OPTION value=\"%s\">%s\n",keep_msg,keep_msg);
      		printf("<OPTION value=\"%s\">%s\n",not_keep_msg,not_keep_msg);
      		printf("</SELECT><TD>\n");
      		printf("</TR>\n");
      		printf("</TABLE>\n");
      		printf("<HR>\n");
      		printf("<TABLE>\n");
      		printf("<TR>\n");
      		printf("<TD>Enable autoReply:</TD>\n");
      		printf("<TD>\n");
      		printf("<SELECT NAME=\"autoreply\">\n");
      		printf("<OPTION value=\"%s\">%s\n",autoreply,autoreply);
      		printf("<OPTION value=\"%s\">%s\n",not_autoreply,not_autoreply);
      		printf("</SELECT>\n");
      		printf("</TD>\n");
      		printf("</TABLE>\n");
      		printf("Autoreply message:<BR>\n");
      		printf("<TEXTAREA NAME=\"autoreply_msg\" ROWS=\"10\" COLS=\"60\">");
      		printf("%s",options[13][1]); 
      		printf("</TEXTAREA>\n");
      		printf("<P>\n");
      		printf("<CENTER>\n");
      		printf("<TABLE>\n");
      		printf("<TR>\n");
      		printf("<TD>\n");
      		printf("<INPUT TYPE=RESET VALUE=\"Clear\">\n");
      		printf("<INPUT TYPE=SUBMIT VALUE=\"Submit\">\n");
      		printf("</TD></TR></TABLE>\n");
      		printf("</CENTER>\n");
      		printf("</FORM>");
      		printf("</BODY>");   
      		fflush(stdout);   
   	}

   }


