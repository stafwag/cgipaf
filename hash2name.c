#include "config.h"
#include "xcrypt.h"

void usage() {

	fprintf(stderr,"hash2name: hash\n");
	exit(1);

} 

int main(int argn, char **argv) {

	char *hash;

	if(argn != 2) {

		usage();

	};

	hash=argv[1];

	printf("%s\n",null2str(hash2name((hash))));

}
