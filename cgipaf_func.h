/*
 * cgipaf_func.h
 *
 * Copyright (C) 2003 Staf Wagemakers Belgie/Belgium
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


#ifndef __CGIPAF_FUNC_H
#define __CGIPAF_FUNC_H
#include "ccgi.h"

char *** add_post_2_string_pair ( WEBDATA *data, char *** ret );
char *   webdata_2_post_string  ( WEBDATA *data );
#endif
