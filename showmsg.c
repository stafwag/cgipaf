/*
 * showmsg.c
 *
 * Copyright (C) 2001 Staf Wagemakers Belgie/Belgium
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
 * update_item	     = set item value to alt_msg ( NULL = no update )
 * 
 * return         0  = file displayed
 *                1  = redirect
 *                2  = alt_msg displayed
 *                3  = no msg displayed, can't load file and alt_msg is NULL
 */
int show_msg(FILE *config_file,char *root,char *section_name,char *msgfile, char *alt_msg,char ***parms, char *update_item)
{
char *cp,*fullname,**cc;
struct stat statbuf;

if (update_item!=NULL) {

	update_string_pair_item(parms,update_item,alt_msg,0);

}

	

/*
 * get directive array
 */
if (section_name==NULL)
   cc=get_global_config_array(config_file,msgfile);
else cc=get_section_config_array(config_file,section_name,msgfile);

/*
 * error handling, if we can't get the directive data show alt_msg
 */
if (config_file==NULL) {
   write_log(LOG_USER,7,"showmsg.c: config_file is NULL, display alt_msg");
   return(show_alt_msg(alt_msg));
}
if (cc==NULL) {
   write_log(LOG_USER,7,"showmsg.c: directive array is NULL (directive not found???), display alt_msg");
   return(show_alt_msg(alt_msg));
}
if (*cc==NULL) {
   write_log(LOG_USER,7,"showmsg.c: directive array is empty, display alt_msg");
   return(show_alt_msg(alt_msg));
}

/*
 * redirect, if the first item of the directive is "redirect" 
 * send redirect header
 */
if ((!strcasecmp(cc[0],"redirect"))&&(cc[1]!=NULL)&&(strlen(cc[1]))) {
   char *location;
   location=add_parms(cc[1],parms);
   if(location==NULL) {
      write_log(LOG_USER,7,"showmsg.c: location is NULL can't redirect, display alt_msg");
      return(show_alt_msg(alt_msg));
   }
   else {
      write_log(LOG_USER,7,"showmsg.c: redirect to %s",location);
      html_redirect(location);
   }
   return(1);
}

/*
 * if root == NULL, display alt_msg
 */
if (root==NULL) {
     write_log(LOG_USER,7,"showmsg.c: root is NULL, display alt_msg");
     return(show_alt_msg(alt_msg));
}

/*
 * create full document name
 */
fullname=(char *) malloc(strlen(root)+strlen(*cc)+1+strlen("/"));
   
if (fullname==NULL) {
   write_log(LOG_USER,7,"showmsg.c: fullname is NULL (malloc error???), display alt_msg");
   return(show_alt_msg(alt_msg));
}
else {
   fullname[0]='\0';
   if(root!=NULL) strcpy(fullname,root);
   strcat(fullname,"/");
   strcat(fullname,*cc);
   write_log(LOG_USER,7,"showmsg.c: fullname set to %s",fullname);
 
   /*
    * Is the document a valid file???
    */
   if(stat(fullname,&statbuf)==-1) {
     write_log(LOG_USER,7,"showmsg.c: stat() error %s, display alt_msg",strerror(errno));
     return(show_alt_msg(alt_msg));
   }
 
 /*
  * Display the document
  */
   printf("Content-type: text/html\n\n");
   
   if(print_phpfile(fullname,parms)==-1) {
      write_log(LOG_USER,7,"showmsg.c: print_phpfile() failed, display alt_msg");
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

/*
 * call show_msg and exit, close config_file if open
 */
void show_msg_and_exit(FILE *config_file,char *root,char *virtual_name,char *msgfile, char *alt_msg,char ***parms, char *update_item)
{
     show_msg(config_file,root,virtual_name,msgfile,alt_msg,parms,update_item);
     if (config_file!=NULL) fclose(config_file);
     exit(0);
}

int show_msgs(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char ***parms,char *update_item)
{
   char **cc;
   int i;
   if(msgfiles==NULL) return(-1);
   for(cc=msgfiles;*cc!=NULL;++cc) {
      i=show_msg(config_file,root,section_name,*cc,NULL,parms,update_item);
      fflush(stdout);
      if(i!=3) break;
   }
   if(i==3) {
      print_txt_msg(alt_msg);
      i=2;
   }
   return(i);
}

void show_msgs_and_exit(FILE *config_file,char *root,char *section_name,char **msgfiles, char *alt_msg,char ***parms, char *update_item)
{
     show_msgs(config_file,root,section_name,msgfiles,alt_msg,parms,update_item);
     if (config_file!=NULL) fclose(config_file);
     exit(0);
}
