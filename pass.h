/*
 * pass.h
 *
 * Copyright (C) 1999,2002 Staf Wagemakers Belgie/Belgium
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

#include "common.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include "md5crypt.h"
#include "salt.h"
#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif

#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif

#include "xmalloc.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <errno.h>

#ifndef __PASS_H
#define __PASS_H

#define SHADOWFILE "/etc/shadow"
#define OSHADOWFILE "/etc/oshadow"
#define TMPFILE "/etc/tmp_passwd"

#ifndef BSDHOST
#define TMPLOCK "/var/lock/cgipaf"
#else
#define TMPLOCK "/var/spool/lock/cgipaf"
#endif

#define BUFFERLEN 256

#ifdef _WITHPAM
#include <security/pam_appl.h>
char *set_pam_service(char *s);
char *last_pam_msg();
int  set_pam_chauth_flag (int flag);
#define PASS_SUCCESS PAM_SUCCESS
#endif
#ifndef _WITHPAM
#include "fgetpwnam.h"
#define PASS_SUCCESS 0
#endif

struct pw_info {
    struct passwd *p;
    struct spwd *sp;
#ifdef _WITHPAM
    pam_handle_t *pamh;
#endif
    };

struct pw_info * get_pw(char *name);
int ckpw(struct pw_info *pw,char *pass);
int chpw(struct pw_info *pw,char *pass);
#ifdef _WITHPAM
int ckpw_nopam(struct pw_info *pw,char *pass);
int chpw_nopam(struct pw_info *pw,char *pass);
struct pw_info * get_pw_nopam(char *name);
#endif
#endif
