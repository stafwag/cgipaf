/*
 * ccgi.h
 *
 * Copyright (C) 1999,2002-04 Staf Wagemakers Belgie/Belgium
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __CCGI_H
#define __CCGI_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xmalloc.h"

#include <stdarg.h>
#include "xstring.h"

typedef struct w {
int n;
char *string;
char **name;
char **value;
} WEBDATA;
WEBDATA *str2webdata(char equals, char end, char *c);
char * get_postitem(WEBDATA *wp,char *name);
char * get_webdata_item(WEBDATA *wp,char *name);
char * get_cookie(WEBDATA *wp,char *name);
WEBDATA *read_post(void);
WEBDATA *read_get(void);
int print_htmlfile (char * filename);
void html_redirect(char *location);
void print_txt_msg(char *msg);
void printf_txt_msg(char *fmt, ...);
void print_html_msg(char *msg);
void printf_html_msg(char *fmt, ...);
WEBDATA *get_cookies(void);
void set_cookie(char *name, char *cookie, char *extra);
char *textarea2asc(char *txt);
void yes_no(char *test,char *name,char *not_name,int *state);

#endif
