/*
 * acl.h                                   (c)2002 Staf Wagemakers
 *                                                 staf@patat.org
 */

#include "config.h"
#include "pass.h"
#include <grp.h>
int memberofgrp (struct pw_info *p,char *grpname);
void set_useraclorder(int i);
int check_useracl(struct pw_info *p,char **users);
int check_grpacl(struct pw_info *p,char **groups);
int get_useraclorder(FILE *config_file,char *section_name);
int get_groupaclorder(FILE *config_file,char *section_name);
int acl (FILE *config_file,char *section_name, struct pw_info *p); 
