#include "../salt.h"
#include "../xstring.h"
#include <stdio.h>
#include <unistd.h>



const char * md5_seed(void);
const char * std_seed(void);


/*
int
pw_gensalt(char *salt, size_t saltlen, const char *type, const char *option)
*/


int main() {

	char *salt=NULL;
	char saltBuf[100];
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

#ifdef NETBSDHOST
	int i;
	char *e; 

	i=pw_gensalt(saltBuf,100,"old");
	printf("old DES %d %s\n",i,null2str(saltBuf));

	i=pw_gensalt(saltBuf,100,"new");
	printf("new DES %d %s\n",i,null2str(saltBuf));

	i=pw_gensalt(saltBuf,100,"md5");
	printf("md5 %d %s\n",i,null2str(saltBuf));
        e=crypt("password1234",crypt_make_salt("MD5",NULL));
	printf("md5 hash %s\n",null2str(e));

	i=pw_gensalt(saltBuf,100,"blowfish");
	printf("blowfish %d %s\n",i,null2str(saltBuf));

	i=pw_gensalt(saltBuf,100,"sha1");
	printf("sha1 %d %s\n",i,null2str(saltBuf));

        e=crypt("password1234",saltBuf);
	printf("sha1 hash %s\n",null2str(e));

	i=pw_gensalt(saltBuf,100,"brol");
	printf("brol %d %s\n",i,null2str(saltBuf));



#endif


}
