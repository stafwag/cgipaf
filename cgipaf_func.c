/*
 * cgipaf_func.c
 *
 * Copyright (C) 2003,04 Staf Wagemakers Belgie/Belgium
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

#include "cgipaf_func.h"

char *** add_webdata_2_string_pair (WEBDATA *data, char *** ret,char *head,char *tail) {
	char *c;
	char *value;

	int i;

	if(data==NULL)  return ret;

	for (i=0;i<data->n;i++) {

		if (data->name[i]) {

			c=xmalloc(strlen(head)+strlen(data->name[i])+strlen(tail)+1);

			strcpy(c,head);
			strcat(c,data->name[i]);
			strcat(c,tail);

			ret=add_2_string_pair(ret,c,data->value[i]);


		}  


	}

	return(ret);


}

int print_strings(char ***str_pair) {

	char ***pt;
	int ret=0;

	if(str_pair==NULL) return(ret);

	for(pt=str_pair;pt[0][0]!=NULL;pt++) {

		printf("%s -> %s\n",pt[0][0],pt[0][1]);
		++ret;

	}

	return(ret);

}

char *** add_get_2_string_pair(WEBDATA *data,char *** ret) {

	char txt_get1[]="_GET[";
	char txt_get2[]="]";

	ret=add_webdata_2_string_pair(data,ret,txt_get1,txt_get2);

	return(ret);

}

char *** add_post_2_string_pair ( WEBDATA *data, char *** ret ) {

	char txt_post1[]="_POST[";
	char txt_post2[]="]";
	char txt_poststring[]="poststring";
	char *c;

	ret=add_webdata_2_string_pair(data,ret,txt_post1,txt_post2);

	c=xmalloc(strlen(txt_poststring)+1);
	strcpy(c,txt_poststring);

	ret=add_2_string_pair(ret,c,data->string);

	return(ret);

}

char *** add_env_2_string_pair ( char *** ret ) {

	char txt_env1[]="_ENV[";
	char txt_env2[]="]";
	extern char **environ;
	char   *c,**cc;
	char   *env_name=NULL,*env_value=NULL;


	for(cc=environ;*cc;cc++) {

		int name_length=0;
		int real_name_length=0;

		c=strchr(*cc,'=');

		if(c==NULL) continue;

		name_length=c-*cc;
		real_name_length=strlen(txt_env1)+strlen(txt_env2)+name_length;
	
		env_name=xmalloc(real_name_length+1);
		env_name[0]='\0';
		strcat(env_name,txt_env1);
		strncat(env_name,*cc,name_length);
		strcat(env_name,txt_env2);

		++c;
		
		env_value=xmalloc(strlen(c)+1);
		strcpy(env_value,c);

		ret=add_2_string_pair(ret,env_name,env_value); 

	}

	return(ret);


}
