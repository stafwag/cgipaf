/*
 * salt.c                                          (c) 2002 Staf Wagemakers
 *                                                    http://staf.patat.org
 *                                                           staf@patat.org
 */

#include "salt.h"

#ifdef FREEBSDHOST
static unsigned char itoa64[] =         /* 0 ... 63 => ascii - 64 */
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
to64(char *s, long v, int n)
{
  while (--n >= 0) {
        *s++ = itoa64[v&0x3f];
        v >>= 6;
        }
}
#endif

/*
 * retruns a random char with valid ascii char's
 *
 * s should be a random integer
 */
int gen_random(int s)
{
int i;
srand(s);
i=rand();
i=i/(RAND_MAX/63);
if(i<0) i=0;
if(i>63) i=63;
if(i<12) return (i+46);
if((i>=12)&&(i<38)) return (i+(65-12));
if(i>=38) return (i+(97-38));
}
#ifdef MD5_CRYPT

/*
 * returns a md5 salt ( based on FreeBSD code )
 */

char * md5_seed(void)
{
struct timeval tv;
int randinit;
static char md5salt[32];

if (!randinit) {
     randinit = 1;
#ifndef FREEBSDHOST
     srandom(getpid());
#else
     srandomdev();
#endif
     
}

gettimeofday(&tv,0);

to64(&md5salt[0], random(), 3);
to64(&md5salt[3], tv.tv_usec, 3);
to64(&md5salt[6], tv.tv_sec, 2);
to64(&md5salt[8], random(), 5);
to64(&md5salt[13], random(), 5);
to64(&md5salt[17], random(), 5);
to64(&md5salt[22], random(), 5);
md5salt[27] = '\0';
return(md5salt);
}

#endif

/*
 * returns a 2 char random salt
 */
char * std_seed(void)
{
static char stdsalt[3];
srand(getpid());
stdsalt[0]=gen_random(time(0)-rand());
stdsalt[1]=gen_random(time(0)+getpid());
stdsalt[2]='\0';
return(stdsalt);
}

