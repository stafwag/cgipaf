#include "config.h"

#include <unistd.h>

#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "salt.h"

int main() {

	char *e;

	e=crypt("password",crypt_make_salt("DES",NULL));

	if (e==NULL) {

		puts("ERROR crypt des failed");

	}

	else {

		puts(e);


	}

	char *mysalt=crypt_make_salt("UNKOWN",NULL);

	if (mysalt!=NULL) {  

		e=crypt("password",crypt_make_salt("UNKOWN",NULL));

		if (e==NULL) {

			printf("ERROR crypt fake failed errno = %d , errstr = %s \n",errno,strerror(errno));

		}

		else {

			puts(e);


		}

	} else { 	

		fprintf(stderr,"Sorry mysalt = NULL - unsupported crypt type? -\n");


	}


	e=crypt("password",crypt_make_salt("MD5",NULL));

	if (e==NULL) {

		printf("ERROR crypt md5 failed errno = %d , errstr = %s \n",errno,strerror(errno));

	}

	else {

		puts(e);


	}

	int sha_rounds=6000;

	e=crypt("password",crypt_make_salt("SHA256",&sha_rounds));

	if (e==NULL) {

		printf("ERROR crypt sha256 failed errno = %d , errstr = %s \n",errno,strerror(errno));

	}

	else {

		puts(e);


	}

	e=crypt("password1234",crypt_make_salt("SHA512",&sha_rounds));

	if (e==NULL) {

		printf("ERROR crypt sha512 failed errno = %d , errstr = %s \n",errno,strerror(errno));

	}

	else {

		puts(e);


	}

}
