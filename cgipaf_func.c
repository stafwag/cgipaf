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
	char txt_post_string[]="post";
	char *c;

	int i;

	if(data==NULL) {
		return ret;
	}


	for (i=0;i<data->n;i++) {

		if (data->name[i]) {

			c=xmalloc(strlen(txt_post1)+strlen(data->name[i])+strlen(txt_post2)+1);

			strcpy(c,txt_post1);
			strcat(c,data->name[i]);
			strcat(c,txt_post2);

			ret=add_2_string_pair(ret,c,data->value[i]);

		}


	}

	/* c=webdata_2_post_string(data);

	ret=add_2_string_pair(ret,txt_post_string,c);

	*/

	return(ret);

}

char * webdata_2_post_string(WEBDATA *data) {

	int i,n;
	int bufferlen=200;
	char *strbuf=xmalloc(bufferlen);

	strbuf[0]='\0';

	if(data==NULL) return(0);

	for (i=0;i<data->n;i++) {
		int prev_size=bufferlen;
		if (data->name[i]) {
			n=strlen(strbuf)+strlen(data->name[i])+strlen("=")+strlen(data->value[i])+strlen("&")+1;

			while (n>bufferlen) {
				bufferlen*=2;
			}

			if(prev_size!=bufferlen) strbuf=xrealloc(strbuf,bufferlen);

			strcat(strbuf,data->name[i]);
			strcat(strbuf,"=");
			strcat(strbuf,data->value[i]);
			strcat(strbuf,"&");
		}
	}

	if(strbuf[strlen(strbuf)-1]=='&') strbuf[strlen(strbuf)-1]='\0';
	strbuf=xrealloc(strbuf,strlen(strbuf)+1);

	return(strbuf);

}

