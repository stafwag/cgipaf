/*
 * xcrypt.h 
 *
 * Copyright (C) 2015 Staf Wagemakers Belgie/Belgium   
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

#include "config.h"

#include "xmalloc.h"
#include "salt.h"

#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include "xstring.h"


char *xsaltbyid (int meth,void *arg);
char *xsalt (const char *meth, void *arg);
char *xcryptbyid(const char *pass, int typeint,void *arg);
char *xcrypt(const char *pass, const char *type,void *arg);
char * crypttype2str(int i);
int cryptstr2int(const char *txt);
int is_md5(char *p);
int is_blowfish(char *p);
int is_sha256(char *p);
int is_sha512(char *p);
int is_des(char *p);
char ** xcrypt_supported_crypts();
int is_crypt_supported(char * hashname);
int hash2crypttype (char *hash);

#ifndef MODERNCRYPT
#include "md5crypt.h"
#endif

#ifdef HAVE_CRYPT_H
#include <crypt.h>
#endif

