/*
 * configfile.c
 *
 * Copyright (C) 1999,2001,2003 Staf Wagemakers Belgie/Belgium 
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include "configfile.h"
#define BUFFER_LEN 256

/*
 *
 * converts line to a string with only the global part
 * current_section = hulp string
 * global = help int
 *
 */

char *only_global(char *line,char **current_section,int *global)
{
char *c,*s=line,*globals,*found,*end_section;
int first=1;
globals=(char *)calloc(strlen(line)+1,sizeof(char *));
end_section=(char *)xmalloc(1);
while(1) {
   if (*global) {
      if((found=strstr(s,"<"))==NULL) {
        strncat(globals,s,found-s);
        break;
      }
      if((c=strstr(found,">"))==NULL) break;
      *global=0;
      strncat(globals,s,(found-s));
      *current_section=(char *)xrealloc(*current_section,(c-found)+1);
      memset(*current_section,'\0',(c-found)+1);
      strncat(*current_section,found+1,c-found-1);
      
      s=c;
   }
   if(*current_section!=NULL) {
      end_section=(char *) xrealloc(end_section,strlen(*current_section)+strlen("</")+strlen(">")+1);
      strcpy(end_section,"</");
      strcat(end_section,*current_section);
      strcat(end_section,">");
  
      if((found=istrstr(s,end_section))==NULL) {
	 break;
      }
      s=found+strlen(end_section);
      *global=1;
   }
   }
return(globals);
}
     
/*
 *
 * reads a variable out the configfile
 * fp           = stream
 * var_name     = variable name
 * section_name = section name
 *                if NULL sections are ignored
 * mode         = 0 -> normal mode
 *                1 -> get only global variables
 * return       = value 
 *              = NULL bij een fout                
 *
 */
char * real_get_config(FILE *fp,char *section_name,char *var_name,int mode)
{

	int l,ii=0;
	int sect=0,global=1;
	char *r=NULL,*c=NULL,*ss=NULL,s[BUFFER_LEN],*start_section=NULL;
	char *end_section=NULL,*current_section=NULL;

	if(var_name==NULL) 
		return(NULL);

	if(fp==NULL) 
		return NULL;

	fseek(fp,0,SEEK_SET);

	if (s==NULL) 
		return(NULL);

	if (section_name!=NULL) {
   		start_section=(char *)xmalloc(strlen(section_name)+1+strlen("<")+strlen(">"));
   		end_section=(char *)xmalloc(strlen(section_name)+1+strlen("</")+strlen(">"));
   		strcpy(start_section,"<");
   		strcat(start_section,section_name);
   		strcat(start_section,">");
   		strcpy(end_section,"</");
   		strcat(end_section,section_name);
   		strcat(end_section,">");
	}
	else 
		sect=1;

	while (fgets(s,BUFFER_LEN,fp)) {

  		if(!(strrchr(s,'\n'))) 
			{errno=E2BIG;return(NULL);} /* line too long */
  
		s[strlen(s)-1]='\0';
  		cut_rem(s);
  		if(mode==1) { 
			ss=only_global(s,&current_section,&global);
		}
    		else ss=s;
  		if(ss==NULL) return (NULL);
  		if (section_name!=NULL) {
     			if (!sect) {
        			if((c=istrstr(ss,start_section))!=NULL) {
          			ss=c;sect=1;
        			}
     			}
 
   			if (sect) {
      				if ((c=istrstr(ss,end_section))!=NULL) {
         			ss=c;sect=0;
      				}
    			}
  		}
  		if ((sect)&&(ss!=NULL)) {
     			c=mv_2_next(ss);
     			l=strlen(var_name);
     			if (get_item_size(c)==l) {
        			if (!strncasecmp(c,var_name,l)) {
	   				c+=l;
	   				c=mv_2_next(c);
           				if (r==NULL) r=(char *)xmalloc(strlen(c)+1);
              					else r=(char *) xrealloc(r,strlen(c)+1);
              				if(r==NULL) break; 
              				strcpy(r,c);
				}
     			}
  		}
	}

	if((mode==1)&&(ss!=NULL)) 
		xfree(ss); 
	if(start_section!=NULL) 
		xfree(start_section); 
	if(end_section!=NULL)
		xfree(end_section);
	if(current_section!=NULL) 
		xfree(current_section);
	return(r);
}

/*
 *
 * get a section variable
 *
 */
char * get_section_config(FILE *fp,char *section_name,char *var_name)
{
return (real_get_config(fp,section_name,var_name,0));
}

/*
 *
 * get a global variable
 *
 */
char* get_global_config(FILE *fp,char *var_name)
{
return(real_get_config(fp,NULL,var_name,1));
}

/*
 *
 * get any variable
 *
 */
char * get_config(FILE *fp,char *var_name)
{
return(get_section_config(fp,NULL,var_name));
}

/*
 *
 * fetch the first part of a variable
 * fp           = stream
 * section_name = section_name
 *                if NULL sections are ignored
 * var_name          = variable name
 * mode         = 0 -> normal mode
 *                1 -> global mode
 * returns char * item 
 *         NULL = error
 *
 */
char * real_get_config_item(FILE *fp,char *section_name,char *var_name,int mode) 
{
char *c;
c=real_get_config(fp,section_name,var_name,mode);

if(c!=NULL) {
   if (c[0]=='\"') {
      rmpos(c,0);
      cut_after_quote(c);
   }
   else {
	   cut_space(c);
   }
   if(strlen(c)==0) {
	   xfree(c);
	   c=NULL;
   }
   else {
   	xrealloc(c,strlen(c)+1); 
   }
}
return(c);
}

/*
 *
 * get a section item
 *
 */
char * get_section_config_item(FILE *fp,char *section_name,char *var_name) 
{
return(real_get_config_item(fp,section_name,var_name,0));
}

/*
 *
 * get any item
 *
 */
char * get_config_item(FILE *fp,char *var_name) 
{
return(get_section_config_item(fp,NULL,var_name));
}

/*
 *
 * get a global item
 *
 */
char * get_global_config_item(FILE *fp,char *var_name)
{
return(real_get_config_item(fp,NULL,var_name,1));
}

/*
 *
 * fetch a variable out the cfg file, and
 * split into an array
 *
 * fp           = stream
 * var_name          = variable name
 * section_name = section name
 *                if NULL sections are ignored
 * mode         = 0 -> normal mode
 *                1 -> global mode
 * returns char ** items array ends with a NULL
 *         NULL = error
 *
 */
char ** real_get_config_array(FILE *fp,char *section_name,char *var_name,int mode)
{
char *c,*c2,**cc;
int i=0,n=0;
c=real_get_config(fp,section_name,var_name,mode);
if (c==NULL) return (NULL);
c2=c;
c2=mv_2_next(c2);
cc=(char **) xmalloc(sizeof(char **)*(n+2));
do {
   c2=mv_2_next(c2);
if (c2[0]=='\"') {
   rmpos(c2,0);
   i=get_quoted_item_size(c2);
   rmpos(c2,i);
   }
   else i=get_item_size(c2);
   cc=(char **) xrealloc(cc,sizeof(char **)*(n+2));
   cc[n]=(char *) xmalloc(i+1);
   if (i) {
      strncpy(cc[n],c2,i);
      cc[n][i]='\0';
      c2=c2+i;
      }
   ++n;
}  while (i);

cc[--n]=NULL;
free(c);
return(cc);
}

/*
 *
 * get a section config array
 *
 */
char ** get_section_config_array(FILE *fp,char *section_name,char *var_name)
{
return(real_get_config_array(fp,section_name,var_name,0));
}

/* 
 *
 * get any config array
 *
 */
char ** get_config_array(FILE *fp,char *var_name)
{
return(get_section_config_array(fp,NULL,var_name));
}

/*
 *
 * get a global config array
 *
 */
char ** get_global_config_array(FILE *fp,char *var_name)
{
return(real_get_config_array(fp,NULL,var_name,1));
}

/*
 *
 * replaces %{variable_name} with a new string
 * 
 * parms[0][0] = variable name , parms[0][1] = value , ...
 *
 */
char * add_parms(char *txt,char ***parms) {
char *ret,*c,*txt2;
c=txt2=replace_headtail_needles(txt,parms,"%{","}");
ret=cut_between(txt2,"%{","}");
free(c);
return(ret);
}

/* ------------------------------------------- */
/* bewaard een variable in een config bestand  */
/*                                             */
/* fp   = stream                               */
/* var_name  = variable                             */
/* value= value                                */
/* return = value                              */
/*        = NULL bij een fout                  */
/* ------------------------------------------- */
char * save_config(FILE *fp,char *var_name,char *value)
{
char *c=(char *)xmalloc(strlen(var_name)+strlen(value)+strlen("\t")+strlen("\n")+1);
if (fp=NULL) return NULL;
strcpy(c,var_name);
strcat(c,"\t");
strcat(c,value);
strcat(c,"\n");
fputs(c,fp);
return(value);
}
char *get_sg_item(FILE *fp,char *section_name,char *var_name)
{
char *ret;
ret=get_section_config_item(fp,section_name,var_name);

if(ret==NULL) {
	ret=get_global_config_item(fp,var_name);
}
   
return(ret);
}

char *get_sg_config(FILE *fp,char *section_name,char *var_name)
{
char *ret;
ret=get_section_config(fp,section_name,var_name);

if(ret==NULL) ret=get_global_config(fp,var_name);

return(ret);
}

char **get_sg_config_array(FILE *fp,char *section_name,char *var_name)
{
char **ret;
ret=get_section_config_array(fp,section_name,var_name);

if(ret==NULL) ret=get_global_config_array(fp,var_name);

return(ret);
}

