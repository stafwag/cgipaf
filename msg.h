#ifndef _CGIPAF_MSG_H
#define _CGIPAF_MSG_H

char err_readdata[]  = "Error reading data, please contact the webmaster\n";
char err_loginname[] = "Can't read loginname\n";
char err_access[]    = "Access denied...\n";
char err_invalid[]   = "Invalid password or username.\n";
char err_newpass[]   = "Can't read new password\n";
char err_match[]     = "They don't match\n";
char err_unchanged[] = "Password unchanged\n";
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
char err_openvacations[] = "Can't open ~/vacations.txt, please contact the webmaster";
char err_updateprocmailrc[] = "Can't update ~/.procmailrc, please contact the webmaster";
char err_cookietimeout[] = "Cookie timeout exceeded...\n";
char err_cracklib[]    =  "BAD PASSWORD:\n";
char err_mailcfgscript[] = "run_mailcfg exits with a non-null value:";
char err_setuidzero[]  =  "script permission problem, please contact the webmaster\n";
char err_cfg[]		= "Configuration error";
char err_useraclorder[]  = " UserAclOrder is invalid\n";
char err_groupaclorder[]  = " GroupAclOrder is invalid\n";
char msg_changed[]   =	"Password changed...\n";
char msg_updated[]   =  "Mail configuration updated...\n";
char *err_chpw[]     ={	"Couldn't open passwd file",                 /* 0 */
			"Couldn't lock passwd file",                 /* 1 */
			"Couldnt't lock lockfile",                   /* 2 */
			"Couldn't open tmp file",                    /* 3 */
			"fileno() failed",                           /* 4 */
			"fchmod() failed",                           /* 5 */
			"bufferlength too small",                    /* 6 */
			"error while writting to tmpfile",           /* 7 */
			"out of memory",                             /* 8 */
			"lsstat() failed",                           /* 9 */
			"rename() failed",                           /* 10 */
			"user not found",                            /* 12 */
                        "unsupported crypt type",                    /* 13 */
		        "pam error",                                 /* 14 */
			"\0","\0","\0","\0","\0"
		      };
char err_mcfg_readname[]    = "[CGIpaf] Can't read loginname\n";
char err_mcfg_configfile[]  = "[CGIpaf] configuration error, can't open";
char txt_contact_webmaster[]= "Please contact the webmaster.\n";
char err_mcfg_accessdb[]    = "[CGIpaf] configuration error, (view)mailcfg needs a accessdb.\n Please contact the webmaster";
char err_badword[]          = "Password uses an illegal word...\n";
char txt_cgipaf[]	    = "CGIpaf";	
char warn_update_accessdb[] = "Warning failed to update";
char warn_root[]            = "Someone try to login as root";
char warn_uid[]             = "Someone try to login as UID ";
char warn_below_minuid[]    = " which is below the min_uid";
char warn_above_maxuid[]    = " which is above the max_uid";

#endif /* _CGIPAF_MSG_H */
