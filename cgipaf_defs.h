#ifndef  _CGIPAF_DEFS_H
#define  _CGIPAF_DEFS_H

#include "config.h"

#ifndef  _WITHPAM
#include "fgetpwnam.h"
#endif

#define  LOGIN                  "name"
#define  ROOT                   "root"
#define  PASSWORD               "passwd"
#define  MINUID                 10
#define  LOGIN_DOCUMENT         "login_document"
#define  ERR_LOGIN              "error_loginname"
#define  ERR_ACCESS             "error_access"
#define  DOC_ROOT               "document_root"
#define  ERR_ACCESSDB		"error_accessdb"
#define  ERR_INVALID            "error_invalid"
#define  ERR_LOCKED             "error_locked"
#define  ERR_FORWARD            "error_readforward"
#define  ERR_FORWARDTO          "error_forwardto"
#define  ERR_INVALIDFORWARDTO   "error_invalidforwardto"
#define  ERR_KEEP_MSG		"error_readkeepmsg"
#define  ERR_AUTOREPLY          "error_readautoreply"
#define  ERR_AUTOREPLYMSG       "error_autoreplymsg"
#define  ERR_COOKIE		"error_cookie"
#define  ERR_COOKIETIMEOUT	"error_cookietimeout"
#define  ERR_DELFORWARD		"error_deldotforward"
#define  ERR_DELPROCMAIL	"error_deldotprocmailrc"
#define  ERR_OPENVACATIONS      "error_openvacations"
#define  ERR_UPDATEPROCMAILRC   "error_updateprocmailrc"
#define  ERR_READNAME           "error_readname"
#define  ERR_CRACKLIB           "error_cracklib"
#define  ERR_PAMERROR           "error_pam"
#define  ERR_BADWORD            "error_illegalword"
#define  CFG_MINUID             "min_uid"
#define  CFG_MAXUID             "max_uid"
#define  CFG_MINLENGTH          "min_length"
#define  CFG_MAXLENGTH          "max_length"
#define  CFG_ACCESSDB           "accessdb"
#define  CFG_MAXINVALID         "max_invalid"
#define  CFG_INVALIDTIMEOUT     "invalid_timeout"
#define  CFG_COOKIETIMEOUT      "cookie_timeout"
#define  CFG_SYSLOG		"syslog"
#define  CFG_LOGLEVEL		"loglevel"
#define  CFG_CRACKLIB		"cracklib"
#define  CFG_PAM_CHANGE_EXPIRED_AUTHTOK	"set_PAM_CHANGE_EXPIRED_AUTHTOK"
#define  CFG_CRACKLIB_DICTPATH	"cracklib_dictpath"
#define  CFG_USE_STATEFILE      "use_statefile"
#define  CFG_ILLEGALWORDS       "illegal_words"
#define  CFG_PASSWD_LOCATION	"passwd_location"
#define  CFG_SHADOW_LOCATION    "shadow_location"
#define  CFG_VMAIL_SUPPORT	"vmail_support"

#define  NEWPASS1 		"newpass1"
#define  NEWPASS2 		"newpass2"
#define  MINLENGTH 		6
#define  MAXLENGTH 		8
#define  MINUID 		10
#define  ERR_NEWPASS 		"error_newpassword"
#define  ERR_MATCH   		"error_match"
#define  ERR_UNCHANGED  	"error_unchanged"
#define  ERR_TOOSHORT 		"error_tooshort"
#define  ERR_TOOLONG  		"error_toolong"
#define  ERR_LOCKED   		"error_locked"
#define  ERR_MAILCFGSCRIPT      "error_mailcfgscript"
#define  CFG_MINUID   		"min_uid"
#define  CFG_MAXUID   		"max_uid"
#define  CFG_MINLENGTH 		"min_length"
#define  CFG_MAXLENGTH 		"max_length"
#define  CFG_MAILCFG            "mailcfg_document"
#define  RUN_SUCCESS            "run_success"
#define  RUN_LOCKED             "run_locked"
#define  RUN_BEFORE_MAILCFG     "run_before_mailcfg"
#define  RUN_AFTER_MAILCFG      "run_after_mailcfg"
#define  RUN_MAILCFG            "run_mailcfg"
#define  CFG_PAM_SERVICE        "pam_service"
#define  CGIPAFSTATEFILE        ".cgipaf_state"

#ifdef   CGIPAF_PASSWD
#define  CFGSECTION             "passwd"
#endif   /* CGIPAF_PASSWD */

#ifdef   CGIPAF_VIEWMAILCFG
#define  CFGSECTION             "mailcfg"
#include "mailconfig.h"
#endif   /* CGIPAF_VIEWMAILCFG */

#ifdef   CGIPAF_MAILCFG
#define  CFGSECTION             "mailcfg"
#include "mailconfig.h"
#endif   /* CGIPAF_MAILCFG */

#include <pwd.h>
#include <stdio.h>
#include <string.h>

#ifdef   HAVE_LIBCRACK
#ifndef  HAVE_CRACK_H
extern char const * FascistCheck(char const password[], char const dictpath[]);
#else
#include <crack.h>
#endif   /* HAVE_CRACK_H */
#endif   /* HAVE_LIBCRACK */

#ifdef   HAVE_CRYPT_H
#include <crypt.h>
#endif   /* HAVE_CRYPT_H */

#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/types.h>

#if HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
# define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
# define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#ifndef LOG_AUTHPRIV
#define LOG_AUTHPRIV LOG_AUTH
#endif  /* LOG_AUTHPRIV */

#include "pass.h"
#include "ccgi.h"
#include "configfile.h"
#include "msg.h"
#include "accessdb.h"
#include "createcookie.h"
#include "xmalloc.h"
#include "out_of_memory.h"

#ifdef NEED_COMPAT_H
#include "compat.h"
#endif /* NEED_COMPAT_H */

#include "mailconfig.h"
#include "showmsg.h"
#include "write_log.h"
#include "acl.h"
#include "cgipaf_func.h"

#endif /* _CGIPAF_DEFS_H */
