/*
 * xmalloc.h					      (c) 2001 Staf Wagemakers
 */
#include <malloc.h>
#ifndef __XMALLOC_H
# define __XMALLOC_H

void *set_memerr(void *s);
void *xmalloc (size_t size);
void *xrealloc (void *ptr, size_t size);

#endif
