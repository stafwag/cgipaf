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


/*

		e=xcrypt("password","DES",NULL);
		printf("DES \"%s\"\n",null2str(e));

		e=xcrypt("password","MD5",NULL);
		printf("MD5 \"%s\"\n",null2str(e));

		e=xcrypt("password","SHA1",NULL);
		printf("SHA1 \"%s\"\n",null2str(e));

		e=xcrypt("password","BLOWFISH",NULL);
		printf("BLOWFISH \"%s\"\n",null2str(e));

		e=xcrypt("password","SHA256",&sha2rounds);
		printf("SHA256 \"%s\"\n",null2str(e));

		e=xcrypt("password","SHA512",&sha2rounds);
		printf("SHA512 \"%s\"\n",null2str(e));

		e=xcrypt("password","FOO",NULL);
		printf("FOO \"%s\"\n",null2str(e));

*/

	int i=0;

	for(i=0;i<11;i++) {

		e=xcryptbyid("password",i,NULL);
		printf("%d \"%s\"\n",i,null2str(e));


	}
}
