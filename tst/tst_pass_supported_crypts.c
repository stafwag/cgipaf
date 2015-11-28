#include "../xcrypt.h"

int main() {

	char **cc;
	int *ii;
	int i;
	int a[20];
	int max;
	int is;
	int *c;

	printf("crypts = \"%s\"\n",string_array_to_str(xcrypt_supported_crypts()));

	ii=xcrypt_supported_crypt_ids();

	fprintf(stderr,"sizeof(ii) = %d , sizeof(int) max = %d\n",sizeof(ii),sizeof(int),max);

	for(c=ii;*c!=-1;c++) {

		printf("*c = %d\n",*c);


	}


	printf("ii=%d max=%d\n",sizeof(ii),max);

	is=is_crypt_supported("boe");
	printf("is_crypt_supported(boe) = %d\n",is);

	is=is_crypt_supported("sha1");
	printf("is_crypt_supported(sha1) = %d\n",is);

	is=is_crypt_supported("sha512");
	printf("is_crypt_supported(sha512) = %d\n",is);

	is=is_crypt_id_supported(4);
	printf("is_crypt_id_supported(4) = %d\n",is);
	is=is_crypt_id_supported(10);
	printf("is_crypt_id_supported(10) = %d\n",is);


}
