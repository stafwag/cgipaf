/*
 * cgipaf_func.c
 *
 * Copyright (C) 2003,2006 Staf Wagemakers Belgie/Belgium
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
#include "write_log.h"

char *** add_post_2_string_pair ( WEBDATA *data, char *** ret ) {

	char txt_post1[]="_POST[";
	char txt_post2[]="]";
	char txt_poststring[]="poststring";
	char *c;

	int i;

	write_log(LOG_USER,7,"cgipaf_func.c: add_post_2_string_pair started...");

	if(data==NULL)  return ret;

	write_log(LOG_USER,7,"cgipaf_func.c: parse post data loop started...");

	if (data->name != NULL ) {

		for (i=0;i<data->n;i++) {

			write_log(LOG_USER,7,"cgipaf_func.c: loop counter %d/%d...",i,data->n);

			if (data->name[i]!=NULL) { 

				write_log(LOG_USER,7,"cgipaf_func.c: processing %s...",data->name[i]);

				c=xmalloc(strlen(txt_post1)+strlen(data->name[i])+strlen(txt_post2)+1);

				strcpy(c,txt_post1);
				strcat(c,data->name[i]);
				strcat(c,txt_post2);

				write_log(LOG_USER,7,"cgipaf_func.c: adding %s to _POST array...",c);

				ret=add_2_string_pair(ret,c,data->value[i]); 

				write_log(LOG_USER,7,"cgipaf_func.c: _POST[%s] assigned ...",c);

			}

			write_log(LOG_USER,7,"cgipaf_func.c: loop ends counter = %d...",i);


		}

	}
	else {

		write_log(LOG_USER,7,"cgipaf_func.c: post array seems to empty...");

	}

	write_log(LOG_USER,7,"cgipaf_func.c: parse post data loop ends...");

	c=xmalloc(strlen(txt_poststring)+1);
	strcpy(c,txt_poststring);

	ret=add_2_string_pair(ret,c,data->string);

	write_log(LOG_USER,7,"cgipaf_func.c: add_post_2_string_pair ends...");

	return(ret);

}

