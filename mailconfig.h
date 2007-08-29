/*
 * mailconfig.h
 *
 * Copyright (C) 2001,2006,2007 Staf Wagemakers Belgie/Belgium
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

#ifndef _CGIPAF_MAILCONFIG_H
#define _CGIPAF_MAILCONFIG_H

#include "common.h"
#include "pass.h"
#include "searchdomain.h"
#include "configfile.h"
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
int write_procmailrchead(struct pw_info *pw, char *sendmail,char *formail);
int save_mailcfg_status(struct passwd *p,int forward,char *forwardto,int keep,int autoreply);
char *get_maildomain(char *domain);
int use_mailcfg_statefile(int setflag);
int get_mailcfg_status(struct pw_info *pw);

#endif
