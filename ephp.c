/*
 * ephp.c     		- emulate php -
 *
 * Copyright (C) 2001,2003,2007 Staf Wagemakers Belgie/Belgium
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

#include "ephp.h"
#include <string.h>

/*
 *
 * replace var with value
 *
 */
char *replace_var(char *str,char *real_name,char *real_value)
{
char txt_null[]="\0";
char *var=real_name;
char *value=real_value;
char *ss,*s,*c,*varname=NULL,*ret=NULL;
int  found;

if(var==NULL) var=txt_null;
if(value==NULL) value=txt_null; 

ret=(char *)xmalloc(sizeof(char *));
varname=(char *)xmalloc(strlen(var)+strlen("$")+1);
strcpy(varname,"$");
strcat(varname,var);
*ret='\0';
c=str;
while(c) {
   ss=NULL;
   found=0;
   s=strstr(c,varname);
   if(s!=NULL) {
      ss=s+strlen(varname);
      if(isspace(*ss)||*ss=='\0'||*ss==';'||*ss=='$') found=1;
   }
   if(!found) {
     ret=(char *)xrealloc(ret,strlen(ret)+strlen(c)+1);
     strcat(ret,c);
     break;
   }
   ret=(char *)xrealloc(ret,strlen(ret)+(s-c)+strlen(value)+1);
   strncat(ret+strlen(ret),c,s-c);
   strcat(ret,value);
   c=ss;
   if((*ss!='$')&&(*ss!='\0')) ++c;
}
free(varname);
return(ret);
}

/*
 *
 * replace all vars in a string
 *
 */
char *replace_vars(char *txt,char ***vars)
{
  int i=0;
  char *txt2,*ret;
  ret=NULL;
  txt2=(char *) xmalloc(strlen(txt)+1);
  strcpy(txt2,txt);
  while (vars[i][0]!=NULL) {
     if(ret!=NULL) free(ret);
     ret=replace_var(txt2,vars[i][0],vars[i][1]);
     if(ret==NULL) break;
     txt2=(char *) xrealloc(txt2,strlen(ret)+1);
     strcpy(txt2,ret);
     i++;
  }
  free(txt2);
  return(ret);
}

/*
 *
 * execute a php command
 *
 */
int ephp_exec(char *command, char *value,char ***values)
{
   char *c;
   c=replace_vars(value,values);
   if(c==NULL) return(-1);
   if(!strcmp(command,"echo")) printf("%s",c);
      else if(!strcmp(command,"include")) { 
             if(print_phpfile(c,values)==-1) {
               printf("<P>failed to include %s</P>",c);
	     }
           }
             
   return(0);
}
   
/*
 *
 * split an instruction in a command and a value
 * and send them to ephp_exec
 *
 */
int ephp_split(char *phpinstr,char ***values)
{
   char *c,*command,*value,*malloc_value;
   size_t size;
   c=mv_2_next(phpinstr);
   size=get_item_size(c);
   if(size==0) return(0);
   command=(char *)xmalloc(size+1);
   strncpy(command,c,size);
   command[size]='\0';
   c=mv_2_next(c+size);
   malloc_value=value=(char *)xmalloc(strlen(c)+1);
   strcpy(value,c);
   cut_space(value);
   if (strlen(value)==0) return(0);
   if (value[0]=='"') {
      ++value;
      if(value[strlen(value)-1]=='"') value[strlen(value)-1]='\0'; 
   } 
   if(ephp_exec(command,value,values)==-1) return(-1);
   free(command); 
   free(malloc_value); 
   return(0);
 
}
         
/*
 *
 * split a php string into instructions and send them to ephp_split
 *
 */
int ephp_parse(char *phpstr,char ***values)
{
   char *s,*php_instr,*buf;
   int i;
   int size;
   php_instr=(char *)xmalloc(sizeof(char *));
   php_instr[0]='\0';
   buf=phpstr;
   do {
      s=strstr(buf,";");
      if(s!=NULL) {
	 php_instr=(char *)xrealloc(php_instr,(s-buf)+1);
	 size=sizeof(php_instr);
	 memset(php_instr,'\0',size);
	 php_instr[s-buf]='\0'; 
	 strncpy(php_instr,buf,s-buf);
      }
      else {
	 php_instr=(char *)xrealloc(php_instr,strlen(buf)+1);
	 strcpy(php_instr,buf); 
      }
      buf=s+1; 
      i=ephp_split(php_instr,values); 
      if(i==-1) return(-1);
   } while (s!=NULL);
   free(php_instr);
   return(0);
}
   
/*
 *
 * displays a php file
 *
 * returns -1 on error
 * 
 */
int print_phpfile (char * filename,char ***values)
{
FILE *fp;
char buffer[500],buffer2[500],*s;
int found=0;
if((fp=fopen(filename,"r"))==NULL) return(-1);

while (fgets(buffer,500,fp)) {

	s=strstr(buffer,"<?");

	if (!found) {

   		if (s==NULL) printf("%s",buffer);

     		else {

          		strncpy(buffer2,buffer,s-buffer);
          		buffer2[s-buffer]='\0';
          		printf("%s",buffer2);
          		found=1;
          		strcpy(buffer2,buffer);
          		strcpy(buffer,buffer2+(s-buffer)+2);

     		}

	}

	if (found) {

   		buffer2[0]='\0';
   		s=strstr(buffer,"?>");

   		if(s!=NULL) {

     			fseek(fp,0-((buffer+strlen(buffer))-s-2),SEEK_CUR);
     			*s='\0'; 
     			found=0;

   		}

   		if(ephp_parse(buffer,values)==-1) {

			fclose(fp);
	 		return(-1);  

   		}

	}
    
}

fflush(stdout);
fclose(fp);
return(0);

}
