#include "config.h"
#include "pass.h"
#include <grp.h>
void set_denyallusers(int i);
void set_denyallgrps(int i);
void set_denyall(int i);
int memberofgrp (struct pw_info *p,char *grpname);
void set_useraclorder(int i);
int check_useracl(struct pw_info *p,char **users);
int check_grpacl(struct pw_info *p,char **groups);
int get_denyallusers(FILE *config_file,char *section);
int get_useraclorder(FILE *config_file,char *section_name);
int get_groupaclorder(FILE *config_file,char *section_name);
int user_acl(FILE *config_file,char *section_name, struct pw_info *p);
int get_denyall(FILE *config_file,char *section_name);
int get_denyallgroups(FILE *config_file,char *section_name);
int group_acl (FILE *config_file,char *section_name, struct pw_info *p); 
