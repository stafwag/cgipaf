#include "fgetpwnam.h"

struct passwd * fgetpwnam(FILE *pw_file,char *name) {
	struct passwd *ret;

	while ((ret=fgetpwent(pw_file))!=NULL) {
		if(strcmp(ret->pw_name,name)==0) break;
	}
	return(ret);

}
#ifdef HAVE_SHADOW_H
struct spwd * fgetspnam(FILE *sh_file,char *name) {
        struct spwd *ret;

        while ((ret=fgetspent(sh_file))!=NULL) {
                if(strcmp(ret->sp_namp,name)==0) break;
	}
	return(ret);

}
#endif

