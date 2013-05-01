/*
 * showmsg.h
 *
 * Copyright (C) 2001,02 Staf Wagemakers Belgie/Belgium
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
