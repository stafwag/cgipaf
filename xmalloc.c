/*
 * xmalloc.c
 *
 * Copyright (C) 2001, 2006, 2012 Staf Wagemakers Belgie/Belgium
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "xmalloc.h"

#ifndef HAVE_MALLOC

     #undef malloc

     #include <sys/types.h>

     void *malloc ();

     /* Allocate an N-byte block of memory from the heap.
        If N is zero, allocate a 1-byte block.  */

     void *
     rpl_malloc (size_t n)
     {
       if (n == 0)
         n = 1;
       return malloc (n);
     }

#endif

#ifndef HAVE_REALLOC

     #undef realloc

     #include <sys/types.h>

     void *realloc ();

     /* Allocate an N-byte block of memory from the heap.
        If N is zero, allocate a 1-byte block.  */

     void *
     rpl_realloc (void *p,size_t n)
     {
       if (n == 0)
         n = 1;
       return realloc (p,n);
     }

#endif



void xmalloc_out_of_memory() {
   fprintf(stderr,"Out of memory\n");
   exit(1);
}
void (*set_memerr(void (*s)()))()
{
static void (*err)()=&xmalloc_out_of_memory;
if (s!=NULL) err=s;
return err;
}
void *xmalloc (size_t size)
{
 register void *value=(void *) malloc(size);
 void (*errfunction)();
 if (value==NULL) {
   errfunction=set_memerr(NULL);
   errfunction(); 
   }
 return (value);
}
void *xrealloc (void *ptr, size_t size)
{
 register void *value=(void *) realloc(ptr,size);
 void (*errfunction)();
 if (value==NULL) {
   errfunction=set_memerr(NULL);
   errfunction(); 
   }
 return (value);
}

void *xcalloc (size_t n, size_t size)
{
 register void *value=(void *) calloc(n,size);
 void (*errfunction)();
 if (value==NULL) {
     errfunction=set_memerr(NULL);
     errfunction();
  }
  return (value);
}

void xfree(void *ptr) 
{
  if(ptr!=NULL) free(ptr);
}
