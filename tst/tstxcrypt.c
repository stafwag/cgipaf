#include "../xcrypt.h"

int main() {

	char *e;
	int sha2rounds=1000000;

	char *txtcrypts[]={"DES","MD5","BLOWFISH","SHA1","SHA256","SHA512","FOO",NULL};

	char **cp;

/*
	cp=txtcrypts[0];
	printf("\"%s:\" \n",cp);
*/

	int counter=0;

	for(cp=txtcrypts;*cp!=NULL;cp++) {

		e=xcrypt("password",*cp,NULL);
		printf("\"%s:\" \"%s\"\n",*cp,null2str(e));

		if(counter > 10) {

			exit(2);

		}

		counter++;

	}

	int i=0;

	for(i=0;i<11;i++) {

		e=xcryptbyid("password",i,NULL);
		printf("%d \"%s\"\n",i,null2str(e));


	}

	fprintf(stderr,"best supported crypt; %s\n",xcrypt_best_supported_crypt());
	// xcrypt_best_supported_crypt_id();
	fprintf(stderr,"best supported crypt id; %d\n",xcrypt_best_supported_crypt_id());
}
