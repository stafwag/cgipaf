#include "config.h"
#include <pwd.h>

#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif

#include <stdio.h>
#include <sys/types.h>

struct passwd * fgetpwnam(FILE *pw_file,char *name);

#ifdef HAVE_SHADOW_H
struct spwd   * fgetspnam(FILE *sh_file,char *name);
#endif
