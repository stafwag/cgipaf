/*
 * tst.c
 * 
 * Copyright (C) 2002 Staf Wagemakers Belgie/Belgium
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

#include <stdio.h>
#include <string.h>
#include "pass.h"
#include "md5crypt.h"
int main (int argc, char **argv) {
char *name, *pass, * new_password, *encrypt_password;
int i;
struct pw_info *pw;
	if (argc!=4) {
		fprintf(stderr,"usage: %s name password newpassword\n",argv[0]);
	        exit(1);
	}
name=argv[1];
pass=argv[2];
new_password=argv[3];

printf("name: %s , pass: %s new_password : %s\n",name,pass,new_password);

pw=get_pw(name);

if (pw==NULL) {
	fprintf(stderr,"user not found\n");
	exit(1);
}
if ((ckpw(pw,pass))!=PASS_SUCCESS) {
	fprintf(stderr,"invalid password\n");
	exit(1);
}
printf("Auth OK\n");
/* encrypt_password=libshadow_md5_crypt(new_password,md5_seed()); */
printf ("md5 password: %s\n",encrypt_password);
i=chpw(pw,new_password);
printf ("chpw() returns %d\n",i);
}
