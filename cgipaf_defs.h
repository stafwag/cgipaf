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
#define  ERR_READNAME           "error_readname"
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
#define  NEWPASS1 		"newpass1"
#define  NEWPASS2 		"newpass2"
#define  MINLENGTH 		6
#define  MAXLENGTH 		8
#define  MINUID 		10
#define  ERR_NEWPASS 		"error_newpassword"
#define  ERR_MATCH   		"error_match"
#define  ERR_TOOSHORT 		"error_tooshort"
#define  ERR_TOOLONG  		"error_toolong"
#define  ERR_LOCKED   		"error_locked"
#define  CFG_MINUID   		"min_uid"
#define  CFG_MAXUID   		"max_uid"
#define  CFG_MINLENGTH 		"min_length"
#define  CFG_MAXLENGTH 		"max_length"
#define  CFG_MAILCFG            "mailcfg_document"
#define  RUN_SUCCESS            "run_success"
#define  RUN_LOCKED             "run_locked"
#ifdef   CGIPAF_PASSWD
#define  CFGSECTION             "passwd"
#endif
#ifdef   CGIPAF_VIEWMAILCFG
#define  CFGSECTION             "mailcfg"
#include "mailconfig.h"
#endif
#ifdef   CGIPAF_MAILCFG
#define  CFGSECTION             "mailcfg"
#include "mailconfig.h"
#endif
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#ifndef LOG_AUTHPRIV
#define LOG_AUTHPRIV LOG_AUTH
#endif
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
#endif
#include "mailconfig.h"
#include "showmsg.h"
#include "write_log.h"

