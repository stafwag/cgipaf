/*
 * fgetpwnam.h
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "common.h"
#include <pwd.h>

#ifdef HAVE_SHADOW_H
#include <shadow.h>
#endif

#include <stdio.h>
#include <sys/types.h>

struct passwd * fgetpwnam(FILE *pw_file,char *name);

#ifdef HAVE_SHADOW_H
struct spwd   * fgetspnam(FILE *sh_file,char *name);
#endif
