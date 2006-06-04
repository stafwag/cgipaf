/*
 * accessdb.h
 *
 * Copyright (C) 2001,2003, 2006 Staf Wagemakers Belgie/Belgium
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
