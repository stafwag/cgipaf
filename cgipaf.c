/*
 * cgiPAF.c     		
 *
 * Copyright (C) 2000-04 Staf Wagemakers Belgie/Belgium
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

#include "cgipaf_defs.h"

main()
{
#include "cgipaf_vars.h"
#include "cgipaf_init.c"

#include "cgipaf_getlogindata.c"

#ifdef CGIPAF_PASSWD

#include "cgipaf_checkpw.c"
   
#endif

#include "cgipaf_auth.c"

#ifdef CGIPAF_PASSWD

#include "cgipaf_updatepw.c"

#endif
#ifdef CGIPAF_VIEWMAILCFG

#include "cgipaf_viewmailcfg.c"
	write_log(LOG_AUTHPRIV,6,"User %s has login successfully",login);
#endif
   if (config_file != NULL) fclose(config_file);
   exit(0);
}
