/*
 * showmsg.h					  (c) 2001,02  Staf Wagemakers
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "configfile.h"
#include "ccgi.h"
#include "write_log.h"
#include "ephp.h"
#include "xstring.h"

int show_msg(FILE *config_file,char *root,char *section_name,char *msgfile, char *alt_msg,char ***parms, char *update_item);
void show_msg_and_exit(FILE *config_file,char *root,char *section_name,char *msgfile, char *alt_msg,char ***parms, char *update_item);
int show_msgs(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char ***parms, char *update_item);
void show_msgs_and_exit(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char ***parms, char *update_item);
