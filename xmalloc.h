/*
 * xmalloc.h
 *
 * Copyright (C) 2001, 2002, 2006 Staf Wagemakers Belgie/Belgium
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

/*
 * some system use malloc.h, most modern unixes have malloc defined in
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

void (*set_memerr(void (*s)()))();
void *xmalloc (size_t size);
void *xrealloc (void *ptr, size_t size);
void *xcalloc (size_t n, size_t size);
void xfree(void *ptr);
void xmalloc_out_of_memory();

#endif
