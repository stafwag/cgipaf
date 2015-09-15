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

char *xsaltbyid (int meth,void *arg) {

	char *salt=NULL;

#ifdef  NETBSD_SHA1
	char saltBuf[100];
#endif

	switch(meth) {

		case 0:
				salt=(char *) crypt_make_salt("DES",NULL);
				break;
		case 1:
				salt=(char *) crypt_make_salt("MD5",NULL);
				break;
		case 2:
				salt=NULL;
				break;
		case 3:
				salt=(char *) crypt_make_salt("SHA256",(int *) arg);
				break;
		case 4:
				salt=(char *) crypt_make_salt("SHA512",(int *) arg);
				break;
#ifdef  NETBSD_SHA1
		case 10:


				if(pw_gensalt(saltBuf,99,"sha1",NULL)==0) {

					salt=xmalloc(strlen(saltBuf)+1);
					strcpy(salt,saltBuf);

				} else {

					salt=NULL;

				};

				break;
#endif
		default:
				salt=NULL;
				break;

	}

	return(salt);


}

char *xsalt (const char *meth, void *arg) {

	int methId;
	char *salt;
#ifdef  NETBSD_SHA1
	char saltBuf[100];
#endif

	if (meth==NULL) {

		 return(NULL);

	}

	methId=cryptstr2int(meth);

	salt=xsaltbyid(methId,arg);

	return(salt);

}

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

   if (strlen(p)<4) return(1);
   if (p[0]=='$' && p[2]=='$') return(0);
   return(1);

}

int hash2crypttype (char *hash) {

	if (hash==NULL) return(-1);
	if (is_des(hash)) return(0);;
	if (is_md5(hash)) return(1);;
	if (is_blowfish(hash)) return(2);;
	if (is_sha256(hash)) return(3);;
	if (is_sha512(hash)) return(4);;

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
 * returns an array with the supported crypt types
 */

char ** xcrypt_supported_crypts() {

#ifdef MODERNCRYPT
#ifdef MODERNCRYPT_SHA2

	static char *ret[]={"des","md5","sha256","sha512",NULL};
#else
	static char *ret[]={"des","md5",NULL};

#endif
#else
#ifdef MD5_CRYPT
#ifdef OPENBSD_BLOWFISH
	static char *ret[]={"des","md5","blowfish",NULL};
#else
	static char *ret[]={"des","md5",NULL};
#endif
#else
	static char *ret[]={"des",NULL};
#endif
#endif

	return(ret);

}

int is_crypt_supported(char * hashname) {

	char **supported=xcrypt_supported_crypts();
	char *cp;

	if(hashname==NULL) return(0);

	for(cp=*supported;cp!=NULL;cp++) {

		if(!strcmp(hashname,cp)) {

			return(1);	
			break;

		}	

	}

	return(0);

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

char *xcryptbyid(const char *pass, int typeint,void *arg) {

	char *ret=NULL;
	char *salt=NULL;

	switch(typeint) {

		case 0:
			salt=xsaltbyid(typeint,NULL);
			if(salt==NULL) return(NULL);
			ret=crypt(pass,salt);
			break;

#ifdef OPENBSD_BLOWFISH

		case 2:

                	ret=xmalloc(255);

                	if (crypt_newhash(pass, "blowfish,8", ret,254)!=0) {

				xfree(ret);
				ret=NULL;

			} else {

				ret=xrealloc(ret,strlen(ret)+1);

			}; 

			break;
                        	
#endif /* OPENBSD_BLOWFISH */

#ifdef MD5_CRYPT
		case 1:
			salt=xsaltbyid(typeint,NULL);
			if (salt==NULL) return(NULL);

#ifndef MODERNCRYPT
			ret=libshadow_md5_crypt(pass,salt);
			break;
#else
			ret=crypt(pass,salt);
			break;

#ifdef MODERNCRYPT_SHA2
		case 3:
		case 4:
			salt=xsaltbyid(typeint,arg);
			if (salt==NULL) return(NULL);

			ret=crypt(pass,salt);
			break;
#endif /* SHA2 */
#endif /* MODERNCRYPT */
#endif /* MD5 */


		default:

			ret=NULL;
			break;


	}

	if(ret==NULL) return(NULL);

	int hashtype;

	hashtype=hash2crypttype(ret);

	if(hashtype!=typeint) return(NULL);

	return(ret);

}


char *xcrypt(const char *pass, const char *type,void *arg) {

	int typeint;

	typeint=cryptstr2int(type);

	return(xcryptbyid(pass,typeint,arg));

}

