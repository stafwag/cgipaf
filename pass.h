/* ------------------------------------------------------------------- */
/* pass.h                                           (GPL) 1999 Belgium */
/*                                                     Staf Wagemakers */
/* ------------------------------------------------------------------- */
#include "config.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <crypt.h>
#include <shadow.h>
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
#define SHADOWFILE "/etc/shadow"
#define OSHADOWFILE "/etc/oshadow"
#define TMPFILE "/etc/tmp_passwd"
#define TMPLOCK "/var/lock/cgipasswd"
#define BUFFERLEN 200
#ifdef _WITHPAM
#include <security/pam_appl.h>
#endif

#ifndef __PASS_H
#define __PASS_H
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