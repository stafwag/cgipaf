/*
 *  common.h
 *
 *  Copyright (C) 2006  Staf Wagemakers Belgie/Belgium
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef __COMMON_H

#define __COMMON_H

#include "config.h"
#include "xmalloc.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef	HAVE_STRINGS_H 
#include <string.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_STDINT_H 

#include <stdint.h>

#else
# if HAVE_SYS_STDINT_H
#  include <sys/stdint.h>
# else
#  if HAVE_INTTYPES_H
#  	include <inttypes.h>
#  else
#    if  HAVE_SYS_INTTYPES_H 
#	include <sys/inttypes.h>
#    endif
# endif /* HAVE_INTTYPES_H */
#endif 	/* HAVE_SYS_STDINT_H */
#endif 	/* HAVE_STDINT_H */


#endif


