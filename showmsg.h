/*
 * showmsg.h					   (c) 2001  Staf Wagemakers
 */
#include "configfile.h"
#include "ccgi.h"

int show_msg(FILE *config_file,char *root,char *section_name,char *msgfile, char *alt_msg,char *parms[][2]);
void show_msg_and_exit(FILE *config_file,char *root,char *section_name,char *msgfile, char *alt_msg,char *parms[][2]);
int show_msgs(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char *parms[][2]);
void show_msgs_and_exit(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char *parms[][2]);
