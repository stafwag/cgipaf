char err_readdata[]  = "Error reading data, please contact the webmaster\n";
char err_loginname[] = "Can't read loginname\n";
char err_access[]    = "Access denied...\n";
char err_invalid[]   = "Invalid password or username.\n";
char err_newpass[]   = "Can't read new password\n";
char err_match[]     = "They don't match\n";
char err_tooshort[]  = "Password too short...\n";
char err_toolong[]   = "Password too long...\n";
char err_locked[]    =  "Maximum number of tries exceeded...\n";
char err_forward[]   =  "Can't read forward...\n";
char err_forwardto[] =  "Can't read forward_to...\n";
char err_invalidforwardto[] = "Invalid forward to...\n";
char err_keep_msg[]  =  "Can't read keep_msg...\n";
char err_autoreply[] =  "Can't read autoreply...\n";  
char err_autoreplymsg[] =  "Can't read autoreply_msg...\n";  
char err_cookie[]    =  "To update your mail configuration your browser need to support cookies...\n";
char err_delforward[] =  "Can't delete, ~/.forward please contact the webmaster";
char err_delprocmail[] = "Can't delete, ~/.procmail please contact the webmaster";
char err_openvacations[] = "Can't open ~/vacations.txt, plaese contact the webmaster";
char err_cookietimeout[] = "Cookie timeout exceeded...\n";
char msg_changed[]   =	"Password changed...\n";
char msg_updated[]   =  "Mail configuration updated...\n";
char *err_chpw[]     ={	"Couldn't open passwd file",
			"Couldn't lock passwd file",
			"Couldnt't lock lockfile",
			"Couldn't open tmp file",
			"fileno() failed",
			"fchmod() failed",
			"bufferlength too small",
			"error while writting to tmpfile",
			"out of memory",
			"lsstat() failed",
			"rename() failed",
			"user not found",
		        "pam error","\0","\0","\0","\0","\0"
		      };
char err_mcfg_readname[]    = "[CGIpaf] Can't read loginname\n";
char err_mcfg_configfile[]  = "[CGIpaf] configuration error, can't open";
char txt_contact_webmaster[]= "Please contact the webmaster.\n";
char err_mcfg_accessdb[]    = "[CGIpaf] configuration error, (view)mailcfg needs a accessdb.\n Please contact the webmaster";
char warn_update_accessdb[] = "Warning failed to update";
