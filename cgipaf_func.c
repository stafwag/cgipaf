/*
 *
 * cgipaf_func.c
 *
 *
 * Copyright (C) 2003 Staf Wagemakers Belgie/Belgium 
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */



#include "cgipaf_func.h"

char *** add_post_2_string_pair ( WEBDATA *data, char *** ret ) {

	char txt_post1[]="_POST[";
	char txt_post2[]="]";
	char txt_poststring[]="poststring";
	char *c;

	int i;

	if(data==NULL)  return ret;


	for (i=0;i<data->n;i++) {

		if (data->name[i]) { 

			c=xmalloc(strlen(txt_post1)+strlen(data->name[i])+strlen(txt_post2)+1);

			strcpy(c,txt_post1);
			strcat(c,data->name[i]);
			strcat(c,txt_post2);

			ret=add_2_string_pair(ret,c,data->value[i]); 

		}  


	}
	c=xmalloc(strlen(txt_poststring)+1);
	strcpy(c,txt_poststring);

	ret=add_2_string_pair(ret,c,data->string);

	return(ret);

}

