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

	int i;
	char buffer[256];

	if(data==NULL) {
		fprintf(stderr,"data is NULL\n");
		return ret;
	}


	for (i=0;i<data->n;i++) {

		if (data->name[i]) {

			char *c;

			snprintf(buffer,255,"%s%s%s",txt_post1,data->name[i],txt_post2);

			c=xmalloc(strlen(buffer)+1);

			strcpy(c,buffer);

			ret=add_2_string_pair(ret,c,data->value[i]);


		}


	}

	return(ret);

}

