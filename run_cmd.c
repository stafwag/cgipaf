/*
 * run_cmd.c					 (c) 2001 Staf Wagemakers
 */
#include "run_cmd.h"
int run_cmd(FILE *config_file,char *section,char *directive,char *vars[][2])
{
   char **cc,*cmd_name=NULL,*c=NULL;
   FILE *cmd=NULL;
   fflush(stdout);
   if(section==NULL) cc=get_global_config_array(config_file,directive);
   else cc=get_section_config_array(config_file,section,directive);
   if(cc==NULL) return(0);
   if(*cc==NULL) return(0);
   cmd_name=add_parms(cc[0],vars);
   stripslahes(cmd_name);
   cmd=popen(cmd_name,"w");
   if(cmd==NULL) 
     {
	printf("warning: can't execute %s %s",*cc,strerror(errno));
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
	pclose(cmd);
     }
   fflush(NULL);
   if(c!=NULL) free(c);
   if(cmd_name!=NULL) free(cmd_name);
   free_null_array(cc);
   return(1);
}
