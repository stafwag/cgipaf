/*
 * run_cmd.h				        (c) 2001,2003 Staf Wagemakers
 */
#ifdef NEED_COMPAT_H
#include "compat.h"
#endif
#include "configfile.h"
int run_cmd(FILE *config_file,char *section,char *directive,char ***vars,int mode);
