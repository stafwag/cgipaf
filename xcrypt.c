/*
 * xcrypt.c
 *
 * Copyright (C) 2015 Staf Wagemakers Belgie/Belgium
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


#include "xcrypt.h"

/*
 * returns 0 if p is no md5 password
 *         1 if p is a  md5 password
 */
int is_md5(char *p) {

   if (p==NULL) return(0);

   if (strncmp(p,"$1$",3) == 0) return(1);
   return(0);
}

/*
 * returns 0 if p is no blowfish password
 *         1 if p is a  blowfish password
 */
int is_blowfish(char *p) {

   if (p==NULL) return(0);
   if (strlen(p)<4) return(0);

   if (strncmp(p,"$2a$",2) == 0) {

	if (*(p+3)=='$') return(1);

   }
   return(0);
}

/*
 * returns 0 if p is no sha256 password
 *         1 if p is a  sha256 password
 */
int is_sha256(char *p) {

   if (p==NULL) return(0);

   if (strncmp(p,"$5$",3) == 0) return(1);
   return(0);
}

/*
 * returns 0 if p is no sha512 password
 *         1 if p is a  sha512 password
 */
int is_sha512(char *p) {

   if (p==NULL) return(0);

   if (strncmp(p,"$6$",3) == 0) return(1);
   return(0);
}

/*
 * returns 0 if p is no des password
 *         1 if p is a  des password
 */
int is_des(char *p) {

   if (p==NULL) return(0);
   if (strlen(p)==0) return(0);
   if (strlen(p)<3) return(0);
   if (p[0] == '*') return(0);
   if (strlen(p)<4) return(1);
   if (is_sha1(p)) return(0);
   if (p[0]=='$' && p[2]=='$') return(0);

   return(1);

}

int is_sha1(char *p) {

   if (p==NULL) return(0);

   if (strncmp(p,"$sha1$",3) == 0) return(1);
   return(0);

}

int hash2crypttype (char *hash) {

	if (hash==NULL) return(-1);
	if (is_des(hash)) return(0);;
	if (is_md5(hash)) return(1);;
	if (is_blowfish(hash)) return(2);;
	if (is_sha256(hash)) return(3);;
	if (is_sha512(hash)) return(4);;
	if (is_sha1(hash)) return(10);;

	return(-1);

}


int cryptstr2int(const char *txt)
{

        char *cryptType;
        int ret=-1;

        if (txt==NULL) return(-1);
        if (strlen(txt)>10) return(-1);

        cryptType=xmalloc(strlen(txt)+1);
        strcpy(cryptType,txt);

        if(strtoupper(cryptType)!=0) {

                xfree(cryptType);
                return(-1);

        }


        if (strcmp(cryptType,"DES") == 0 ) ret=0;
        if (strcmp(cryptType,"MD5") == 0 ) ret=1;
        if (strcmp(cryptType,"BLOWFISH") == 0) ret=2;
        if (strcmp(cryptType,"SHA256") == 0) ret=3;
        if (strcmp(cryptType,"SHA512") == 0) ret=4;
        if (strcmp(cryptType,"SHA1") == 0) ret=10;

        xfree(cryptType);

        return(ret);

}


/*
 * return a string with the CRYPTTYPE
 */

char * crypttype2str(int i) 
{

	switch(i) {

		case 0:
			return("DES");
			break;
		case 1:
			return("MD5");
			break;
		case 2:
			return("BLOWFISH");
			break;
		case 3:
			return("SHA256");
			break;
		case 4:
			return("SHA512");
			break;
		case 10:
			return("SHA1");
			break;

		default:
			return("UNKNOWN");
			break;

	}

	return("UNKNOWN");

}


char * hash2name(char *hash) {

	int i=hash2crypttype(hash);

	if(i==-1) {

		return(NULL);

	}

	return(crypttype2str(i));

}

