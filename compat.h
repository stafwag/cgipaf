/*
 * compat.h
 *
 * Copyright (C) 2001,2003 Staf Wagemakers Belgie/Belgium
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
#ifdef HAVE_SYS_SYSTEMINFO_H
#include <sys/systeminfo.h>
#endif
#ifndef HAVE_UNSETENV
void unsetenv(char *env_name);
#endif
#ifndef HAVE_SETENV
int setenv(const char *name, const char *value, int overwrite);
#endif
#ifndef HAVE_GETDOMAINNAME
char *getdomainname(); 
#endif
#ifndef HAVE_GETHOSTNAME
char *gethostname();
#endif
