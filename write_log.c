/*
 * write_log.c
 *
 * Copyright (C) 2001, 2007 Staf Wagemakers Belgie/Belgium
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

#include "write_log.h"
int enable_log(int s)
{
static int log=1;
if (s!=-1) log=s;
return log;
}
unsigned set_loglevel(int l)
{
static unsigned level=6;
if (l!=-1) level=l;
if (level>7) level=7;
return level;
}

void write_log(int facility, int priority, char *fmt, ...)
{
  va_list ap;
  char txt_unknown[] ="unknown";
  char buf[128];
  char prefix[32];
  char clientmsg[96];
  char syslog_msg[256];
  char *clientip;

  /*
  int  loglevels[]={LOG_EMERG,LOG_ALERT,LOG_CRIT,LOG_ERR,LOG_WARNING,LOG_NOTICE,LOG_INFO,LOG_DEBUG};
	*/
  if(!enable_log(-1)) return;
  if(priority>set_loglevel(-1)) return;
  memset(syslog_msg,'\0',sizeof(syslog_msg));
  openlog("CGIpaf",LOG_PID|LOG_CONS,facility);
  switch (priority)
     {
      case 7:
	strcpy(prefix,"DEBUG: ");
	break;
      case 6:
	strcpy(prefix,"INFO: ");
	break;
      case 5:
	strcpy(prefix,"NOTICE: ");
	break;
      case 4:
	strcpy(prefix,"WARNING: ");
	break;
      case 3:
	strcpy(prefix,"ERROR: ");
	break;
      case 2:
	strcpy(prefix,"CRITICAL: ");
	break;
      case 1:
	strcpy(prefix,"ALERT: ");
	break;
      case 0:
	strcpy(prefix,"EMERG: ");
	break;
      default:
	prefix[0]='\0';
     }
  va_start(ap, fmt);
  vsnprintf(buf,127,fmt,ap);
  va_end(ap);
  clientip=getenv("REMOTE_ADDR");
  if (clientip==NULL) clientip=txt_unknown;
  snprintf(clientmsg,95,"[ client %s ] ",clientip);
  strncpy(syslog_msg,prefix,255);
  strncat(syslog_msg,clientmsg,255-strlen(syslog_msg));
  strncat(syslog_msg,buf,255-strlen(syslog_msg));
  syslog(priority,syslog_msg);
  closelog();
}
  
