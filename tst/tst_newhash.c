#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main() {

	char myHash[1024];
	int i;

     	i=crypt_newhash("FooBar123", "blowfish,a", myHash,1024);

     	if (i == 0) {

		fprintf(stderr,"ok %s\n",myHash);
		exit(0);

	};

	fprintf(stderr,"failed %d %s\n",errno,strerror(errno));


}


