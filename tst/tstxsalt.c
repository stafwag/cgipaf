#include "../xcrypt.h"
#include "../xstring.h"

int main() {

	char *salt;
	int sha2rounds=10000000;

	salt=NULL;

	salt=xsalt("DES",NULL);
	printf("des salt=%s\n",null2str(salt));
	salt=xsalt("MD5",NULL);
	printf("md5 salt=%s\n",null2str(salt));
	salt=xsalt("SHA1",&sha2rounds);
	printf("sha1 salt=%s\n",null2str(salt));
	salt=xsalt("SHA256",&sha2rounds);
	printf("sha256 salt=%s\n",null2str(salt));
	salt=xsalt("SHA512",&sha2rounds);
	printf("sha512 salt=%s\n",null2str(salt));

}
