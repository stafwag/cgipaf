/* ---------------------------------------------------------------- */
/* ccgi.h                                     (GPL) Staf Wagemakers */
/* ---------------------------------------------------------------- */
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
int print_htmlfile (char * filename);
void html_redirect(char *location);
void print_txt_msg(char *msg);
void printf_txt_msg(char *fmt, ...);
void print_html_msg(char *msg);
void printf_html_msg(char *fmt, ...);
WEBDATA *get_cookies(void);
void set_cookie(char *name, char *cookie, char *extra);
char *textarea2asc(char *txt);

#endif
