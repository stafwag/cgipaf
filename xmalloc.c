/*
 * xmalloc.c					(c) 2001  Staf Wagemakers
 */

#include "xmalloc.h"

void xmalloc_out_of_memory() {
   fprintf(stderr,"Out of memory\n");
   exit(1);
}
void *set_memerr(void *s)
{
static void *err=xmalloc_out_of_memory;
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
