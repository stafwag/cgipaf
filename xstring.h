/*
 * xstring.h
 *
 *  Copyright (C) 2001, 2006, 2020  Staf Wagemakers Belgie/Belgium
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "common.h"
#include <stdlib.h>
#include <string.h>
void cut_space (char *c);
void cut_after_quote(char *c);
void cut_after_char(char *c, char ch);
void cut_rem (char *c);
char * mv_2_next (char *c);
int get_item_size (char *c);
int get_quoted_item_size (char *c);
int get_char_item_size (char *c,char ch);
void free_null_array (char **cc);
char * replace_headtail_needle(char *txt, char *needle1, char *replace,char *head,char *tail);
char * replace_needle(char *txt, char *needle, char *replace);
char * replace_headtail_needles(char *txt, char ***needles,char *head,char *tail);
char * replace_needles(char *txt, char ***needles);
char * cut_between(char *txt,char *head, char *tail);
int strtoupper(char *str);
int strtolower(char *str);
#ifndef HAVE_STRCASESTR
char *strcasestr(char *haystack, char *needle);
#endif
void rmchar(char *str,char rmc);
void rmpos (char *c,unsigned i);
void stripslahes (char *c);
int is_var_yes(char *var);
void free_string_array (char **array);
int number_of_strings (char **array);
int copy_string_array_pointers (char **dest, char **src);
char * string_array_to_str(char **array);
char *** add_2_string_pair (char ***str_array,char *str1, char *str2);
int number_of_string_pairs (char *** str_pair);
char * get_string_pair_item(char *** str_pair,char *item);
int update_string_pair_item(char ***str_pair,char *item, char *value, int free_mode);
int delete_string_pair_item(char ***str_pair,char *item,int free_mode);
char * clone_string(char *str);
void free_string_pair (char *** str_pair );
unsigned utf8_strsize(char *c);
void replace_char(char *str,char src,char dest);
char * null2str(char *str);
char * utf8_firstchar(char *c);
unsigned utf8_strlen(char *str);
char * right_align_2_strings(char * str1, char * str2, int width);
char * str_nchars(int n, char c);
char ** combine_string_array_pointers( char **src1, char **src2);
char * combine_2_strings(char *str1,char *str2);
char * combine_strings(char **strs);
int isstrdigit(char *str);
int isbin(unsigned char c);
