/*
 * run_cmd.c					 (c) 2001 Staf Wagemakers
 * returns  value  -1: error
 *                 -2: command not defined in cfg file
 *                 stat_val
 */
#include "run_cmd.h"
int run_cmd(FILE *config_file,char *section,char *directive,char *vars[][2])
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
