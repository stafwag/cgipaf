/*
 * xmalloc.h				      (c) 2001,2002 Staf Wagemakers
 */

/*
 * some system use mallo.h, most modern unixes have malloc defined in
 * stdlib.h
 */


#ifdef STDC_HEADERS
#include <stdlib.h>
#else
#ifdef HAVE_MALLOC_H
#include <malloc.h>
#else
#include <stdlib.h>
#endif
#endif

#ifndef __XMALLOC_H

#include <stdio.h>

# define __XMALLOC_H

void *set_memerr(void *s);
void *xmalloc (size_t size);
void *xrealloc (void *ptr, size_t size);
void *xcalloc (size_t n, size_t size);
void xfree(void *ptr);

#endif
