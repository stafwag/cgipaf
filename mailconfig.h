/*
 * mailconfig.h					 (c) 2001 Staf Wagemakers
 */

#ifndef _CGIPAF_MAILCONFIG_H
#define _CGIPAF_MAILCONFIG_H

#include "pass.h"
#include "searchdomain.h"
#include "xmalloc.h"
#include "configfile.h"
#include <string.h>
char *add2home(struct passwd *p,char *dir);
int get_reply(struct pw_info *pw);
char *get_vacations(struct pw_info *pw);
char *get_forward(struct pw_info *pw);
char *get_kforward(struct pw_info *pw);
char *get_mailcfg_forward(struct pw_info *pw);
char *get_mailcfg_kforward(struct pw_info *pw);
int get_mailcfg_reply(struct pw_info *pw);
int enable_reply(struct pw_info *pw,char *domain);
int enable_forward(struct pw_info *pw,char *mailadres,char *domain);
int enable_kforward(struct pw_info *pw, char *mailadres,char *domain);
int tst_emailaddress(char *emailaddress);
int write_procmailrchead(struct pw_info *pw, char *sendmail);
int save_mailcfg_status(struct passwd *p,int forward,char *forwardto,int keep,int autoreply);

#endif
