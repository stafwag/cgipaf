#include "md5.h"

char * libshadow_md5_crypt(const char *pw, const char *salt);
void to64(char *s, unsigned long v, int n);
