/*
 * accessdb.h					  (c) 2001 Staf Wagemakers
 */
#include "config.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef HAVE_GDBM_NDBM_H
#include <gdbm-ndbm.h>
#endif

#ifdef HAVE_GDBMDIR_NDBM_H
#include <gdbm/ndbm.h>
#endif

#ifdef HAVE_NDBM_H
#include <ndbm.h>
#endif

#ifdef HAVE_DB1_NDBM_H
#include <db1/ndbm.h>
#endif

#ifdef HAVE_DB_H_DB_DBM_HSEARCH
#define DB_DBM_HSEARCH 1
#include <db.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif

#include <fcntl.h>
#include "xmalloc.h"
#define  ACCESSDB "/tmp/access"
#define  ACCESS_NL 19
struct access_data {
      time_t ti;
      int  status;                   /* 0 = ok */
      char cookie[60];
   };
int get_access_status (char *accessdb, char *loginname, int max_invalid, int delay);
int save_access_status (char *accessdb, char *loginname, int status, int delay,char *cookie);
int cmp_access_cookie(char *accessdb, char *loginname, char *cookie,time_t timeout);
