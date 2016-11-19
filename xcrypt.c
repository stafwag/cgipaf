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

/*
 * returns a random salt
 */

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
   if (strlen(p)==0) return(0);
   if (strlen(p)<3) return(0);
   if (p[0] == '*') return(0);
   if (strlen(p)<4) return(1);
   if (is_sha1(p)) return(0);
   if (is_blowfish(p)) return(0);
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
 * set the supported behaviour
 *
 * 0 -> work with all supported crypts
 * 1 -> work with only passwd support crypt (default)
 */

int xcrypt_set_supported_crypts_type(int crypt_type) {

        static int xcrypt_supported_crypt_type=1;

        if (crypt_type != -1 ) {

                xcrypt_supported_crypt_type=crypt_type;

        }

        return xcrypt_supported_crypt_type;


}


/*
 * returns an array with the supported crypt types
 */

char ** xcrypt_supported_crypts() {

	char **ret=NULL;

#ifdef MODERNCRYPT

#ifdef MODERNCRYPT_SHA2 /* SHA2 */
	static char *supported_crypts[]={"des","md5","sha256","sha512",NULL};
	static char **passwd_supported_crypts=supported_crypts;
#else
#ifdef  NETBSD_SHA1 /* NETBSD */
	static char *supported_crypts[]={"des","md5","sha1",NULL};
	static char **passwd_supported_crypts=supported_crypts;
#else
	static char *supported_crypts[]={"des","md5",NULL};
	static char **passwd_supported_crypts=supported_crypts;
#endif	            /* NETBSD */	
#endif	            /* SHA2 */	
#else
#ifdef MD5_CRYPT /* MD5 */
#ifdef OPENBSD_BLOWFISH /* OPENBSD */
	
	static char *supported_crypts[]={"des","md5","blowfish",NULL};
	static char *passwd_supported_crypts[]={"blowfish",NULL};
#else
	static char *supported_crypts[]={"des","md5",NULL};
	static char **passwd_supported_crypts=supported_crypts;
#endif			/* OPENBSD */
#else
	static char *supported_crypts[]={"des",NULL};
	static char **passwd_supported_crypts=supported_crypts;
#endif		    /* MD5 */
#endif  /* MODERNCRYPT */

	switch(xcrypt_set_supported_crypts_type(-1)) {

		case 0:
			ret=supported_crypts;
			break;

		case 1:
			ret=passwd_supported_crypts;
			break;

		default:
			ret=supported_crypts;
			break;

	}

	return(ret);

}

char * xcrypt_best_supported_crypt() {

	char **supported=xcrypt_supported_crypts();
	char **cp;
	static char *ret=NULL;

	if(ret!=NULL) return(ret);

	for(cp=supported;*cp!=NULL;cp++) {

		ret=*cp;

	}

	return(ret);
}

int xcrypt_best_supported_crypt_id() {

	char *best=xcrypt_best_supported_crypt();

	return(cryptstr2int(best));

}

int * xcrypt_supported_crypt_ids() {

	char **supported=xcrypt_supported_crypts();
	char **cp;
	static int  *ret=NULL;
	int  size,length;
	int  id;

	if(ret!=NULL) return(ret);

	length=1;

	for(cp=supported;*cp!=NULL;cp++) {
		
		size=length*sizeof(int);
		id=cryptstr2int(*cp);

		ret=xrealloc(ret,size);
		ret[length-1]=id;

		if(length>10) return NULL;

		++length;

	}

	size=length*sizeof(int);
	ret=xrealloc(ret,size);
	ret[length-1]=-1;

	return(ret);

}

int is_crypt_id_supported(int id) {


	int *ids=xcrypt_supported_crypt_ids();
	int *c;

	for(c=ids;*c!=-1;c++) {

		if(id==*c) return(1);

	}

	return(0);

}

int is_crypt_supported(char * hashname) {

	char **supported=xcrypt_supported_crypts();
	char **cp;

	if(hashname==NULL) return(0);

	for(cp=supported;*cp!=NULL;cp++) {

		if(!strcmp(hashname,*cp)) {

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

#ifdef  NETBSD_SHA1 /* NETBSD */
		case 10:
			salt=xsaltbyid(typeint,arg);
			if (salt==NULL) return(NULL);

			ret=crypt(pass,salt);
			break;
#endif

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

	if(hashtype!=typeint) {


 		return(NULL);

	}

	return(ret);

}

char * hash2name(char *hash) {

	int i=hash2crypttype(hash);

	if(i==-1) {

		return(NULL);

	}

	return(crypttype2str(i));

}

char *xcrypt(const char *pass, const char *type,void *arg) {

	int typeint;

	typeint=cryptstr2int(type);

	return(xcryptbyid(pass,typeint,arg));

}

