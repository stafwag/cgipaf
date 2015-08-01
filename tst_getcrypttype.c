#include "pass.h"
#include <pwd.h>
struct pw_info * get_pw_nopam(char *name);


int main() {


	int i;
	struct pw_info *pw=get_pw_nopam("staf");
	if (pw==NULL) {

		fprintf(stderr,"DEBUG: pw is NULL errno = %d %s\n",errno,strerror(errno));
	};

	fprintf(stderr,"DEBUG get_crypttype started\n");
	i=get_crypttype(pw);
	fprintf(stderr,"i=%d type=%s\n",i,crypttype2str(i));


}
