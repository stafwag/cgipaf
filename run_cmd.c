/*
 * run_cmd.c
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 *
 * returns  value  -1: error
 *                 -2: command not defined in cfg file
 *                 stat_val
 *
 */
#include "run_cmd.h"
int run_cmd(FILE *config_file,char *section,char *directive,char ***vars,int mode)
{
   char **cc,*cmd_name=NULL,*c=NULL;
   FILE *cmd=NULL;
   int  ret=-2;
   fflush(stdout);
   if(section==NULL) cc=get_global_config_array(config_file,directive);
   else cc=get_section_config_array(config_file,section,directive);
   if(cc==NULL) return(ret);
   if(*cc==NULL) return(ret);
   cmd_name=add_parms(cc[0],vars);
   stripslahes(cmd_name);
   if(mode==1) setenv("SCRIPT_FILENAME",cmd_name,1);
     else if (mode==2) unsetenv("SCRIPT_FILENAME");
   cmd=popen(cmd_name,"w");
   ret=1;
   if(cmd==NULL) 
     {
	fprintf(stderr,"warning: can't execute %s popen failed %s",*cc,strerror(errno));
	return(-1);
     }
   else 
     {
	if(cc[1]!=NULL) {
	   stripslahes(cc[1]);
	   c=add_parms(cc[1],vars);
	   fprintf(cmd,"%s\n",c);
	   fflush(cmd);
	}
	if((ret=pclose(cmd))==-1) {
	   fprintf(stderr,"warning: can't execute %s pclose failed %s",*cc,strerror(errno));
	}
     }
   fflush(NULL);
   if(c!=NULL) free(c);
   if(cmd_name!=NULL) free(cmd_name);
   free_null_array(cc);
   return(ret);
}
