/* ------------------------------------------------------------------------ */
/* configfile.h                            (GPL) 1999,2001  Belgie  Belgium */
/*                                                          Staf Wagemakers */
/* ------------------------------------------------------------------------ */

#ifndef _STAF_CONFIGFILE_H
#define _STAF_CONFIGFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "xstring.h"

char * get_config(FILE *fp,char *var_name);
char * get_config_item(FILE *fp,char *var_name);
char ** get_config_array(FILE *fp,char *var_name);

char * get_section_config(FILE *fp,char *virtual_name,char *var_name);
char * get_section_config_item(FILE *fp,char *virtual_name,char *var_name);
char ** get_section_config_array(FILE *fp,char *virtual_name,char *var_name);

char * get_global_config(FILE *fp,char *var_name);
char * get_global_config_item(FILE *fp,char *var_name);
char ** get_global_config_array(FILE *fp,char *var_name);

char * save_config(FILE *fp,char *var_name,char *value);
char * add_parms (char *txt , char ***parms);

char * get_sg_item(FILE *fp,char *section_name,char *var_name);
char * get_sg_config(FILE *fp,char *section_name,char *var_name);
char ** get_sg_config_array(FILE *fp,char *section_name,char *var_name);

#endif /* _STAF_CONFIGFILE_H */
