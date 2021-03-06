/*
 * salt.c
 *
 * Copyright (C) 2002, 2007, 2013 Staf Wagemakers Belgie/Belgium
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "salt.h"

#ifdef FREEBSDHOST
static unsigned char itoa64[] =         /* 0 ... 63 => ascii - 64 */
        "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
void to64(char *s, long v, int n)
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

return(i);

}
#ifdef MD5_CRYPT

/*
 * returns a md5 salt ( based on FreeBSD code )
 */

char * md5_seed(void)
{
struct timeval tv;
static char md5salt[32];

#ifndef FREEBSDHOST
     srandom(getpid());
#else
     srandomdev();
#endif
     
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

