/*
 * pass.h					 (GPL) 1999,2002 Belgium
 *                                                     staf wagemakers
 */

#include "config.h"
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

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif

#include <errno.h>

#ifndef __PASS_H
#define __PASS_H

#define SHADOWFILE "/etc/shadow"
#define OSHADOWFILE "/etc/oshadow"
#define TMPFILE "/etc/tmp_passwd"

#ifndef FREEBSDHOST
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
#endif
