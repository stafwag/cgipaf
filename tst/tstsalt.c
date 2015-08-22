#include "../salt.h"
#include "../xstring.h"
#include <stdio.h>

const char * md5_seed(void);
const char * std_seed(void);

int main() {

	char *salt=NULL;
	int sha_rounds=5000;
	salt=null2str(crypt_make_salt("MD5",NULL));
	printf("new salt md5: %s %d\n",salt,strlen(salt));

	salt=null2str(crypt_make_salt("DES",NULL));
	printf("new salt DES: %s %d\n",null2str(salt),strlen(salt));
/*
	salt=md5_seed();
	printf("old salt md5: %s %d\n",null2str(salt),strlen(salt));

	salt=std_seed();
	printf("old salt DES: %s %d\n",null2str(salt),strlen(salt));

*/

	salt=null2str(crypt_make_salt("BLOWFISH",&sha_rounds));
	printf("new salt BLOWFISH: %s %d\n",salt,strlen(salt));

	salt=null2str(crypt_make_salt("SHA256",&sha_rounds));
	printf("new salt SHA256: %s %d\n",salt,strlen(salt));

	salt=null2str(crypt_make_salt("SHA512",&sha_rounds));
	printf("new salt SHA512: %s %d\n",salt,strlen(salt));

}
