/*
 *
 * xstring.c  	- Tired of writing these things over and over again -
 *
 *
 * Copyright (C) 2001,2003 Staf Wagemakers Belgie/Belgium 
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 */

#include "xstring.h"
/*
 *
 * replaces the first ch in c with a '\0', if ch == '\0' 
 * the first space is replaced                                  
 *
 */
void cut_after_char (char *c,char ch)
{
do {
   if (ch=='\0') {
      if (isspace(*c)) { 
         *c='\0';
         break;
	 }
      }
      else {
         if (*c==ch) { 
            *c='\0';
	    break;
	 }
      }
   ++c;
   } while (*c);
}

/*
 *
 * replaces the first quote ('\"') with a '\0'
 *
 */
void cut_after_quote (char *c) 
{
char *cp;
do {
   if(*c=='\"') {
      if(c==cp) 
	{
	   *c='\0';
	   break;
	}
      if(*(c-1)!='\\')
	{
	   *c='\0';
	   break;
	}
   }
++c;
} while(*c);
}

/*
 *
 * get the size of a item
/*
 */
int get_quoted_item_size (char *c)
{
char *cc=c;
int i;
for(;*c!='\0';c++) {
  if (*c=='\"') 
     {
	if(c==cc) break;
	if(*(c-1)!='\\') break;
     } 
};
return(c-cc);
}
   
   
/*
 *
 * removes everything after a space           
 *
 */
void cut_space (char *c)
{
cut_after_char(c,'\0');
}

/*
 *
 * removes everything after a #                
 *
 */
void cut_rem (char *c)
{
cut_after_char(c,'#');
}

/*
 *
 * move to the next item                       
 *
 */
char * mv_2_next (char *c)
{
do if (isspace(*c)) ++c; else break; while (*c);
return c;
}

/*
 *
 * get the size of a item ch=IFS
 *
 */
int get_char_item_size (char *c,char ch)
{
char *cc=c;
int i;
do if ((*c!=ch)&&(*c!='\0')) ++c; else break; while (*c);
return(c-cc);
}

/*
 *
 * get the size of a item
 *
 */
int get_item_size (char *c)
{
char *cc=c;
int i;
do if (!isspace(*c)&&(*c!='\0')) ++c; else break; while (*c);
return(c-cc);
}


/*
 *
 * frees an array of strings
 *
 */
void free_null_array (char **cc)
{
char ** cc2;
cc2=cc;
while (*cc2) { free(*cc2);cc2++; }
free(cc);
}

/*
 * 
 * replaces a needle with a new string head & tail are added to needle 
/*
 */
char * replace_headtail_needle(char *txt, char *needle1, char *replace,char *head,char *tail)
{
   char *c,*s,*ret;
   char *needle=xmalloc(strlen(needle1)+strlen(head)+strlen(tail)+1);
   strcpy(needle,head);
   strcat(needle,needle1);
   strcat(needle,tail);
   c=txt;                                   /* start from the beginning */
   ret=(char *) xmalloc(sizeof(char));
   ret[0]='\0';                             /* start with a empty string */
   while(1) {
      s=strstr(c,needle);    
      if (s==NULL) {
	 ret=(char *) xrealloc(ret,strlen(ret)+strlen(c)+1);
	 strcat(ret,c);
	 break;                             /* needle not found, so we can have a break() */
      }
      ret=(char *) xrealloc(ret,strlen(ret)+(s-c)+strlen(replace)+1);
      strncat(ret+strlen(ret),c,s-c);
      strcat(ret,replace);
      c=s+strlen(needle);
   }
  return(ret);
}

/*
 * 
 * replace needle with a new string
 *
 */
char * replace_needle(char *txt, char *needle, char *replace)
{
   return(replace_headtail_needle(txt,needle,replace,"",""));
}

/*
 *
 * replaces an array of head-tail needles
 * needles[0][0] = needle1 , needles[0][1] = value1 , ...
 *
 */
char * replace_headtail_needles(char *txt, char ***needles,char *head,char *tail)
{
   int i=0,first=1;
   char *txt2,*ret;
   ret=NULL;
   txt2=(char *) xmalloc(strlen(txt)+1);
   strcpy(txt2,txt);
   while (needles[i][0]!=NULL) {
      if(ret!=NULL) free(ret);
      fprintf(stderr,"DEBUG: current needle: %s\n",needles[i][0]);
      ret=replace_headtail_needle(txt2,needles[i][0],needles[i][1],head,tail);
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
 * replaces an array of needles
 * needles[0][0] = needle1 , needles[0][1] = value1 , ...
 *
 */
char * replace_needles(char *txt, char ***needles)
{
   return(replace_headtail_needles(txt,needles,"",""));
}

/*
 *
 * kill all animals with head and tail.
 * !!!! txt be will freed !!!!!
 *
 */
char * real_cut_between(char *txt,char *head, char *tail) {
   char *ret,*s,*c;
   ret=(char *) xmalloc(sizeof(char));
   ret[0]='\0';                           /* start with an empty string */
   c=txt;                                 /* start from the beginning (where else?? */
   s=strstr(c,head);
      if (s==NULL) {
	 return(txt);                          /* No heads anymore */
      }
   ret=(char *) xrealloc(ret,strlen(ret)+s-c+1);
   ret[strlen(ret)+(s-c)]='\0';
   strncat(ret+strlen(ret),c,s-c);    /* ret is the part before the head */
   c=s+strlen(head);
   s=strstr(c,tail);                  /* Where is the tail??? */
   if (s!=NULL) {;                /* doesnt seems to end so bye,bye */
   c=s;
   c+=strlen(tail);                   /* cut the tail */
   ret=(char *) xrealloc(ret,strlen(ret)+strlen(c)+1);
   strcat(ret,c);
   }
                                         /* Look for more animals  */
   return(real_cut_between(ret,head,tail));
}

/*
 *
 * delete all animals with head and tail.
 *
 */
char * cut_between(char *txt,char *head, char *tail) 
{
   char *txt2;
   txt2=xmalloc(strlen(txt)+1);          
   strcpy(txt2,txt);                    /* clone txt, the clone will die */
   return(real_cut_between(txt2,head,tail));
}

/*
 *
 * str to lowercase
 *
 */
int strtolower(char *str) {
   char *c;
   if ( str == NULL ) return(-1);
   c=str;
   while (*c) {
      *c=tolower(*c);
      ++c;
   }
   return 0;
}

/*
 *
 * str to UPPERCASE
 *
 */
int strtoupper(char *str) {
   char *c;
   if ( str == NULL ) return(-1);
   c=str;
   while (*c) {
      *c=toupper(*c);
      ++c;
   }
   return 0;
}

/*
 *
 * ignore case strstr
 *
 */
char * istrstr(char *haystack, char *needle) {
   char *ihaystack, *ineedle;
   char *c,*r=NULL;
   int l;
   ihaystack=xmalloc(strlen(haystack)+1);
   ineedle=xmalloc(strlen(needle)+1);
   
   strcpy(ihaystack,haystack);
   strcpy(ineedle,needle);
   strtolower(ihaystack);
   strtolower(ineedle);
   c=strstr(ihaystack,ineedle);
   if (c!=NULL) {
      l=c-ihaystack;
      r=haystack+l;
   }

   free(ihaystack);
   free(ineedle);
   return(r);
}
void rmchar(char *str,char rmc) {
char *c,*cc;

   for( c=str;*c!='\0';c++) {  
      while(*c==rmc) {
	 for(cc=c;*cc!='\0';cc++) *cc=cc[1];
      }
   }
   str[strlen(str)]='\0';
}

/*
 *
 * Verwijderen v/e karakter in een string
 * char     *c -> string
 * unsigned i  -> positie in string
 *
 * P.S. Deze funktie schuift de kar's ook
 * naar rechts!
 * 
 */
void rmpos (char *c,unsigned i)
{
   unsigned x;
   for (x=i;x<strlen(c);x++) c[x]=c[x+1];
   c[strlen(c)]='\0';
}

void stripslahes (char *c) 
{
for(;*c!='\0';c++) {
   if(*c=='\\') {
      rmpos(c,0);
      switch(*c) {
       case 'n': *c='\n';
	         break;
       case 't': *c='\t';
	         break;
       case 'a': *c='\a';
	         break;
       case 'b': *c='\b';
	         break;
       case 'f': *c='\f';
	         break;
       case 'r': *c='\r';
                 break;
       case 'v': *c='\v';
	         break;
      }
   }
}

}

/*
 * returns 1 is var is "yes"|"on"|"1"
 * returns 0 is var is "no"|"off"|"0"
 * returns -1 is undefined
 */
int is_var_yes(char *var)
{
    char *yes[]={"on","yes","1",NULL};
    char *no[]={"no","off","0",NULL};
    char **ccp;
    if(var==NULL) return(-1);
    for(ccp=yes;*ccp!=NULL;ccp++) 
       if(!strcasecmp(var,*ccp)) return(1);
    for(ccp=no;*ccp!=NULL;ccp++) 
       if(!strcasecmp(var,*ccp)) return(0);
    return(-1);
}

void free_string_array (char **array) {
	char **ccp;
	if(array==NULL) return;
	ccp=array;
	for(;;) {
	     if(ccp!=NULL) {
		if(*ccp!=NULL) {
	        xfree(*ccp);
	        ccp++;
		continue;
		}
	     }
	     break;
	}

	xfree(array);
}

int number_of_strings (char **array) {
	char **ccp;
	int ret=0;
	if(array==NULL) return(-1);
	for(ccp=array;*ccp!=NULL;ccp++) ++ret;
	return(ret);
}

int copy_string_array_pointers (char **dest, char **src) {
	char **ccp;
	char **ccp2;
	
	if(dest==NULL) return(-1);
	if(src==NULL)  return(-1);

	ccp2=dest;
	
	for(ccp=src;*ccp!=NULL;ccp++) { 
		*ccp2++=*ccp; 
	}

	return(0);
}

char *** add_2_string_pair (char ***str_array,char *str1, char *str2) {

	int n;
	char ***ret;

	if(str_array!=NULL) {

		n=number_of_string_pairs(str_array);
		ret=(char ***) xrealloc(str_array,(n+2)*sizeof(char **));
		ret[n+1]=xcalloc(2,sizeof(char *));

	}

	else { 

		ret=(char ***) xcalloc(2,sizeof(char **));
		ret[0] = xcalloc(2,sizeof(char *));
		ret[1] = xcalloc(2,sizeof(char *));

		n = 0;

	}

	ret[n][0]=str1;
	ret[n][1]=str2;

	return(ret);


}

int number_of_string_pairs (char ***str_pair) {

	char ***pt;
	int ret=0;

	for(pt=str_pair;**pt!=NULL;pt++) ret++;

	return(ret);

}

