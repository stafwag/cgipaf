/*
 * showmsg.c					(c) 2001 Staf Wagemakers
 */
#include "showmsg.h"
int show_alt_msg(char *alt_msg) 
{
   if (alt_msg!=NULL) {
      print_txt_msg(alt_msg);
      return(2);
   }
   return(3);

}
   
/*
 * Display a page out the cfg file
 *
 * configfile        = filehandle
 * root              = documentroot
 * section_name      = configuration section
 *                     if NULL it looks in the global section
 * msgfile           = cfg var name
 * alt_msg           = alt msg if the cfg var doesnt esists
 *                     if NULL no msg is displayed
 * options           = ePHP / redirect variabels
 * 
 * return         0  = file displayed
 *                1  = redirect
 *                2  = alt_msg displayed
 *                3  = no msg displayed, can't load file and alt_msg is NULL
 */
int show_msg(FILE *config_file,char *root,char *section_name,char *msgfile, char *alt_msg,char *parms[][2])
{
char *cp,*fullname,**cc;
if (section_name==NULL)
   cc=get_global_config_array(config_file,msgfile);
else cc=get_section_config_array(config_file,section_name,msgfile);
if ((config_file==NULL)||(cc==NULL)) return(show_alt_msg(alt_msg));
if (*cc==NULL) return(show_alt_msg(alt_msg));

if ((!strcasecmp(cc[0],"redirect"))&&(cc[1]!=NULL)&&(strlen(cc[1]))) {
   char *location;
   location=add_parms(cc[1],parms);
   if(location==NULL) return(show_alt_msg(alt_msg));
   else html_redirect(location);
   return(1);
}
   
if (root==NULL) return(show_alt_msg(alt_msg));
   
fullname=(char *) malloc(strlen(root)+strlen(*cc)+1+strlen("/"));

   
if (fullname==NULL) return(show_alt_msg(alt_msg));
else {
   fullname[0]='\0';
   if(root!=NULL) strcpy(fullname,root);
   strcat(fullname,"/");
   strcat(fullname,*cc);
   printf("Content-type: text/html\n\n");
   
   if(print_phpfile(fullname,parms)==-1) {
      if(alt_msg!=NULL) {
	 puts(alt_msg);
	 return(2);
      }
      else {
           puts("print_php_file failed()");
      }
   }
   }
   return(0);
}

void show_msg_and_exit(FILE *config_file,char *root,char *virtual_name,char *msgfile, char *alt_msg,char *parms[][2])
{
     show_msg(config_file,root,virtual_name,msgfile,alt_msg,parms);
     if (config_file!=NULL) fclose(config_file);
     exit(0);
}

int show_msgs(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char *parms[][2])
{
   char **cc;
   int i;
   if(msgfiles==NULL) return(-1);
   for(cc=msgfiles;*cc!=NULL;++cc) {
      i=show_msg(config_file,root,section_name,*cc,NULL,parms);
      fflush(stdout);
      if(i!=3) break;
   }
   if(i==3) {
      print_txt_msg(alt_msg);
      i=2;
   }
   return(i);
}
void show_msgs_and_exit(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char *parms[][2])
{
     show_msgs(config_file,root,section_name,msgfiles,alt_msg,parms);
     if (config_file!=NULL) fclose(config_file);
     exit(0);
}
