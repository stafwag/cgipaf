/*
 * xmalloc.c
 *
 * Copyright (C) 2001 Staf Wagemakers Belgie/Belgium
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
