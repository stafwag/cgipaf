/*
 * acl.h
 *
 * Copyright (C) 2002 Staf Wagemakers Belgie/Belgium
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

#include "common.h"
#include "pass.h"
#include <grp.h>
int memberofgrp (struct pw_info *p,char *grpname);
void set_useraclorder(int i);
int check_useracl(struct pw_info *p,char **users);
int check_grpacl(struct pw_info *p,char **groups);
int get_useraclorder(FILE *config_file,char *section_name);
int get_groupaclorder(FILE *config_file,char *section_name);
int acl (FILE *config_file,char *section_name, struct pw_info *p,char *usermaildomain);
int get_aclorder(FILE *config_file,char *section_name);
