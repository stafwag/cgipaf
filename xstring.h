/*
 * string.h					(c) 2001 Staf Wagemakers
 */
#include "xmalloc.h"
#include <string.h>
void cut_space (char *c);
void cut_after_quote(char *c);
void cut_after_char(char *c, char ch);
char * mv_2_next (char *c);
int get_item_size (char *c);
int get_quoted_item_size (char *c);
int get_char_item_size (char *c,char ch);
void free_null_array (char **cc);
char * replace_headtail_needle(char *txt, char *needle1, char *replace,char *head,char *tail);
char * replace_needle(char *txt, char *needle, char *replace);
char * replace_headtail_needles(char *txt, char *needles[][2],char *head,char *tail);
char * replace_needles(char *txt, char *needles[][2]);
char * cut_between(char *txt,char *head, char *tail);
int strtoupper(char *str);
int strtolower(char *str);
char *istrstr(char *haystack, char *needle);
void rmchar(char *str,char rmc);
void rmpos (char *c,unsigned i);
void stripslahes (char *c);
int is_var_yes(char *var);
