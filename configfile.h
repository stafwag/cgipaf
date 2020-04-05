/*
 * configfile.h
 *
 * Copyright (C) 1999, 2001, 2006, 2020  Staf Wagemakers Belgie/Belgium
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef _STAF_CONFIGFILE_H
#define _STAF_CONFIGFILE_H

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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

char ** get_next_config(FILE *fp);
char ** get_next_config_array(FILE *fp);
char ** get_next_config_var_array(FILE *fp, char *var_name);

#endif /* _STAF_CONFIGFILE_H */
