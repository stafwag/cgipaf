#include "write_log.h"
int enable_log(int s)
{
static int log=1;
if (s!=-1) log=s;
return log;
}

void write_log(int facility, int priority, char *fmt, ...)
{
  va_list ap;
  int d;
  char txt_unknown[] ="unknown";
  char buf[128];
  char prefix[32];
  char clientmsg[96];
  char syslog_msg[256];
  char *clientip;
  char c, *p, *s;
  if(!enable_log(-1)) return;
  memset(syslog_msg,'\0',sizeof(syslog_msg));
  openlog("CGIpaf",LOG_PID|LOG_CONS,facility);
  switch (priority)
     {
      case LOG_DEBUG:
	strcpy(prefix,"DEBUG: ");
	break;
      case LOG_INFO:
	strcpy(prefix,"INFO: ");
	break;
      case LOG_NOTICE:
	strcpy(prefix,"NOTICE: ");
	break;
      case LOG_WARNING:
	strcpy(prefix,"WARNING: ");
	break;
      case LOG_ERR:
	strcpy(prefix,"ERROR: ");
	break;
      case LOG_CRIT:
	strcpy(prefix,"CRITICAL: ");
	break;
      case LOG_ALERT:
	strcpy(prefix,"ALERT: ");
	break;
      case LOG_EMERG:
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
  
