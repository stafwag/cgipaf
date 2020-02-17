/*
 * xstring.c  	- Tired of writing these things over and over again -
 *
 * Copyright (C) 2001,2015,2020  Staf Wagemakers Belgie/Belgium
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

#include "xstring.h"
/*
 * replaces the first ch in c with a '\0', if ch == '\0' 
 * the first space is replaced                                  
 */
void cut_after_char (char *c,char ch)
{
if (c==NULL) return;
if(strlen(c)==0) return;
for(;*c!='\0';c++) {
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
   }
}

/*
 *
 * replaces the first quote ('\"') with a '\0'
 *
 */
void cut_after_quote (char *c) 
{
char *cp=c;

if (c==NULL) return;

for(;*c!='\0';c++) {
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
}

}

/*
 * get the size of a item
 */
int get_quoted_item_size (char *c)
{
char *cc=c;
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
 * removes everything after a space           
 */
void cut_space (char *c)
{
cut_after_char(c,'\0');
}
/*
 * removes everything after a #                
 */
void cut_rem (char *c)
{
cut_after_char(c,'#');
}
/*
 * move to the next item                       
 */
char * mv_2_next (char *c)
{
if (c==NULL) return NULL;
do if (isspace(*c)) ++c; else break; while (*c);
return c;
}

/*
 * get the size of a item ch=IFS
 */
int get_char_item_size (char *c,char ch)
{
char *cc=c;
do if ((*c!=ch)&&(*c!='\0')) ++c; else break; while (*c);
return(c-cc);
}

/*
 * get the size of a item
 */
int get_item_size (char *c)
{
char *cc=c;
do if (!isspace(*c)&&(*c!='\0')) ++c; else break; while (*c);
return(c-cc);
}


/*
 * frees an array of strings
 */
void free_null_array (char **cc)
{
char ** cc2;
if (cc==NULL) return;
cc2=cc;
while (*cc2) { free(*cc2);cc2++; }
free(cc);
}

/*
 * replaces a needle with a new string head & tail are added to needle 
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
 * replace needle with a new string
 */
char * replace_needle(char *txt, char *needle, char *replace)
{
   return(replace_headtail_needle(txt,needle,replace,"",""));
}

/*
 * replaces an array of head-tail needles
 * needles[0][0] = needle1 , needles[0][1] = value1 , ...
 */
char * replace_headtail_needles(char *txt, char ***needles,char *head,char *tail)
{
   int i=0;
   char *txt2,*ret;
   ret=NULL;
   txt2=(char *) xmalloc(strlen(txt)+1);
   strcpy(txt2,txt);
   while (needles[i][0]!=NULL) {
      if(ret!=NULL) free(ret);
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
 * replaces an array of needles
 * needles[0][0] = needle1 , needles[0][1] = value1 , ...
 */
char * replace_needles(char *txt, char ***needles)
{
   return(replace_headtail_needles(txt,needles,"",""));
}

/*
 * kill all animals with head and tail.
 * !!!! txt be will freed !!!!!
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
 * delete all animals with head and tail.
 */
char * cut_between(char *txt,char *head, char *tail) 
{
   char *txt2;
   txt2=xmalloc(strlen(txt)+1);          
   strcpy(txt2,txt);                    /* clone txt, the clone will die */
   return(real_cut_between(txt2,head,tail));
}

/*
 * convert str to lower case
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
 * convert str to UPPERCASE
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

#ifndef HAVE_STRCASESTR
/*
 * like strstr but ignore case
 * Please use strcasestr if you can
 */
char * strcasestr(char *haystack, char *needle) {
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

   xfree(ihaystack);
   xfree(ineedle);
   return(r);
}
#endif

/*
 * removes all rmc chars from string
 */
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
 * Deletes a char in a string
 * char     *c -> string
 * unsigned i  -> position
 *
 * This wil shift all chars to the right
 */
void rmpos (char *c,unsigned i)
{
   unsigned x;
   for (x=i;x<strlen(c);x++) c[x]=c[x+1];
   c[strlen(c)]='\0';
}

/*
 * removes all back slashes from a string
 */
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
 * returns 1 if var is "yes"|"on"|"1"
 * returns 0 if var is "no"|"off"|"0"
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

/*
 * frees a NULL terminated string array
 */
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

/*
 * number of string in a NULL terminated string array
 */
int number_of_strings (char **array) {
	char **ccp;
	int ret=0;
	if(array==NULL) return(-1);
	for(ccp=array;*ccp!=NULL;ccp++) ++ret;
	return(ret);
}

/*
 * copy the string pointers from src to dest
 */
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

char ** combine_string_array_pointers( char **src1, char **src2) {

  char ** dest;

  int length_src1=number_of_strings(src1);
  int length_src2=number_of_strings(src2);

  if ( (length_src1<0) || (length_src2<0) ) return NULL;

  dest=xcalloc(number_of_strings(src1)+number_of_strings(src2), sizeof(char *));
  copy_string_array_pointers(dest,src1);
  copy_string_array_pointers(dest+(length_src1-1),src2);

  return dest;

}

/*
 *
 * add item to a string pair
 *
 */
char *** add_2_string_pair (char ***str_array,char *str1, char *str2) {

	int n;
	char ***ret;

	if(str_array!=NULL) {

		n=number_of_string_pairs(str_array);

		ret=(char ***) xrealloc(str_array,(n+2)*sizeof(char **));
		ret[n]=xmalloc(2*sizeof(char *));
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

/*
 *
 * get the number of string pair array
 *
 */
int number_of_string_pairs (char ***str_pair) {

	char ***pt;
	int ret=0;

	if(str_pair==NULL) return(0);
	
	for(pt=str_pair;pt[0][0]!=NULL;pt++) ret++;

	return(ret);

}

char * get_string_pair_item(char ***str_pair,char *item) {
	char ***pt;
	char *ret=NULL;

	if(str_pair==NULL) return(NULL);

	for(pt=str_pair;pt[0][0]!=NULL;pt++) {

		if(!strcmp(pt[0][0],item)) {
			ret=pt[0][1];
			break;
		}

	}

	return(ret);

}

int update_string_pair_item(char ***str_pair,char *item, char *value, int free_mode) {

	char ***pt;
	int ret=0;

	if(str_pair==NULL) return(-1);
	
	for(pt=str_pair;pt[0][0]!=NULL;pt++) {

		if(!strcmp(pt[0][0],item)) {
			ret=1;

			if(free_mode) xfree(pt[0][1]);

			pt[0][1]=value;
			break;
		}

	}

	return(ret);


}

int delete_string_pair_item(char ***str_pair,char *item,int free_mode) {

	char ***pt;
	int ret=0;

	if(str_pair==NULL) return(-1);
	
	for(pt=str_pair;pt[0][0]!=NULL;pt++) {

		if(!strcmp(pt[0][0],item)) {

			char ***pt2;

			if(free_mode) {
				xfree(pt[0][0]);
				xfree(pt[0][1]);
			}


			for(pt2=pt;pt2[0][0]!=NULL;pt2++) {

				pt2[0][0]=(pt2+1)[0][0];
				pt2[0][1]=(pt2+1)[0][1];

			}

			ret=1;

			break;
		}

	}

	return(ret);

}

void free_string_pair (char ***str_pair) {

	char ***pt;

	if(str_pair==NULL) return;

	for(pt=str_pair;pt[0][0]!=NULL;pt++) {
		xfree(pt[0][0]);
		xfree(pt[0][1]);
		xfree(pt);
		}

	xfree(str_pair);

}

char * clone_string(char *str) {


	char *ret=NULL;

	if(str==NULL) return(ret);

	ret = xmalloc(strlen(str)+1);
	strcpy(ret,str);

	return(ret);

}

void replace_char(char *str,char src,char dest) {

        char *cp;

        for(cp=str;*cp!=0;cp++) {

                if (*cp == src) {

                        *cp=dest;

                }

        }

}

char * null2str(char *str) {

	if(str==NULL) return("null");
	return(str);

}

int isstrdigit(char *str) {

	char *cp;

	for(cp=str;*cp!=0;cp++) {

		if(!isdigit(*cp)) return(0);

	}

	return(1);

}

/*
 * return 0 if c is \n or bacspace
 *          if c is no acscii
 */
int isbin(unsigned char c)
{
if (c==10||c==8) return 0;
if ((c<32)||(c>126&&c<160)) return 1;
  else return 0;
}

/*
 * calculate the char length of a utf8 string
 */
unsigned utf8_strlen(char *str) {

  unsigned u=0;
  unsigned size=strlen(str);
  unsigned pointer=0; 
  char *c;
  c=str;

  while (*c) {

      pointer=pointer+utf8_strsize(c);

      if (*c == 0x08 ) {
        if (u) --u;
      } else {
        ++u;
      }

      if(pointer>size) return u;
      c=str+pointer;

  }

  return u;
}

/*
 * returns a string with the first utf8 char
 */
char * utf8_firstchar(char *c) {

  char *str;
  int   number_of_chars=utf8_strsize(c);

  str=xcalloc(number_of_chars + 1, sizeof(char));

  if ( (number_of_chars==1) && isbin(*c)) {
    str[0]='.';
  } else {
    strncpy(str,c,number_of_chars);
  }

  return(str);

}

/*
 * calcuates the byte size for utf8 string
 */
unsigned utf8_strsize(char *c) {

  int   number_of_chars=0;

  switch (*c) {
    case 0x00 ... 0x7F:
      number_of_chars=1;
      break;
    case 0xffffffC0 ... 0xffffffDF:
      number_of_chars=2;
      break;
    case 0xffffffE0 ... 0xffffffEF:
      number_of_chars=3;
      break;
    case 0xffffffF0 ... 0xffffffF7:
      number_of_chars=4;
      break;
    default:
      number_of_chars=1;
      break;
  }

  return(number_of_chars);

}

/*
 * returns a string with str1 to the left side
 * str to the right size aligned to width
 */

char * right_align_2_strings(char * str1, char * str2, int width) {
  char *ret=NULL;
  int number_of_spaces=0;
  int total_string_length=strlen(str1)+strlen(str2);

  if (width < total_string_length) return NULL;

  number_of_spaces=width-total_string_length;
  if (number_of_spaces < 0) return NULL;

  ret=xmalloc(strlen(str1)+strlen(str2)+number_of_spaces+1);
  if (sprintf(ret,"%s%*c%s",str1,number_of_spaces,' ',str2) < 0) return NULL;
  return ret;

}

/*
 * combines 2 string
 */
char * combine_2_strings(char *str1, char *str2) {

  char *ret=NULL;
  if ( ( str1 == NULL ) || ( str2 ==NULL ) ) return NULL;

  ret=xcalloc(strlen(str1)+strlen(str2), sizeof(char));
  strcpy(ret,str1);
  strcat(ret,str2);

  return ret;
}

/*
 * combines an array of strings
 */
char * combine_strings(char **strs) {

  char *ret=xcalloc(1,sizeof(char));
  char **cc=NULL;

  for(cc=strs;*cc!=NULL;cc++) {

    ret=xrealloc(ret,strlen(ret)+strlen(*cc)+1);
    strcat(ret,*cc);

  }

  return(ret);

}

/*
 * return a string  n * c
 */

char * str_nchars(int n, char c) {
  char *ret=xcalloc(n+1,sizeof(char));
  int i=0;
  for (i=0;i<n;i++) {
    ret[i]=c;
  }
  return ret;
}

