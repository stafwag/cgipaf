#include "ccgi.h"
/* -------------------------------------------------------------------- */
/* ccgi.c                             (GPL) 1999,2001 Staf Wagemakers   */
/* -------------------------------------------------------------------- */
WEBDATA *str2webdata(char equals, char end, char *c)
{
WEBDATA *wp;
char *tmp=NULL;
int t;
wp=(WEBDATA *) xmalloc(sizeof(WEBDATA));
wp->n=0;
wp->name=NULL;
wp->value=NULL;

t=0;
while (1) {

if (tmp==NULL) {
   tmp=(char *)xmalloc(1);
   tmp[0]='\0';
}

if (*c!=equals&&*c!=end&&*c!='\n'&&*c!='\0') {
   tmp=(char *)xrealloc(tmp,t+2);
   tmp[t]=*c; 
   tmp[++t]='\0';
   }
   else {
      if (*c==equals) {
         if (tmp) {
	    wp->name=(char **) xrealloc(wp->name,sizeof(char *)*wp->n+1);
	    wp->name[wp->n]=(char *)xmalloc(strlen(tmp)+1);
	    strcpy(wp->name[wp->n],tmp);
	    }
	 }
	 else {
	     if (strlen(tmp)) {
	        wp->value=xrealloc(wp->value,sizeof(char *)*wp->n+1);
	        wp->value[wp->n]=xmalloc(strlen(tmp)+1);
	        strcpy(wp->value[wp->n],tmp);
	        ++wp->n;
		}
	 }
      t=0;free(tmp);tmp=NULL;
   };
 if (!*c) break;
 ++c;
 }
return wp;
}

/* --------------------------------------------------- */
/* reads a item in wp				       */
/* name = itemname				       */
/* returns :           string                          */
/*                     0 = not found                   */
/* --------------------------------------------------- */

char *get_webdata_item (WEBDATA *wp,char *name)
{
int i,b=0;
for (i=0;i<wp->n;i++) {
    if (wp->name[i]) {
       if (!strcmp(wp->name[i],name)) {
          b=1;
          break;
          }
       }
    }
if(!b) return(NULL);
return(wp->value[i]);
}

/*
 * only exits for compatible with previous versions of ccgi
 */
char * get_postitem(WEBDATA *wp,char *name)
{
   return(get_webdata_item(wp,name));
}

char * get_cookie(WEBDATA *wp,char *name)
{
   return(get_webdata_item(wp,name));
}

/* --------------------------------------------------- */
/* Reads data passed to a cgi - program  	       */
/*						       */
/* returns :           data                            */
/*                     0 = error                       */
/* --------------------------------------------------- */

WEBDATA *read_post(void)
{
WEBDATA *wp;
int bytes=0;
char *c;
c=getenv("REQUEST_METHOD");
if (c==0) return(NULL);
if (strcmp(c,"POST")) return(NULL);
c=getenv("CONTENT_LENGTH");
if(c==0) return(NULL);
sscanf(c,"%d",&bytes);
++bytes;
c=(char *)xmalloc(bytes+1);
fgets(c,bytes,stdin);
wp=str2webdata('=','&',c);
if(wp==NULL) return NULL;
free(c);
return wp;
}

/* -------------------------------------------------------------------
 * displays a html file
 * 
 * returns -1 no error
 * ------------------------------------------------------------------- */
int print_htmlfile (char * filename)
{
    FILE *fp;
    char buffer[500];
    if((fp=fopen(filename,"r"))==NULL) return(-1);

    printf("Content-type: text/html\n\n");

    while (fgets(buffer,500,fp)) {
       printf("%s",buffer);
    }
    fflush(stdout);
    return(0);
}

/* ---------------------------------------------------------------------
 * redirects to a url
 * --------------------------------------------------------------------- */
void html_redirect(char *location)
{
   printf("Status: 302\r\n");
   printf("Location: %s",location);
   fflush(stdout);
}

/* ---------------------------------------------------------------------
 * prints a string as plain text
 * --------------------------------------------------------------------- */
void print_txt_msg(char *msg)
{
   printf("Content-type: text/plain\n\n");
   if (msg!=NULL) {
       puts(msg);
       }
   return;
   fflush(stdout);
}
/* ----------------------------------------------------------------------
 * prints a string a html
 * ---------------------------------------------------------------------- */
void print_html_msg(char *msg)
{
   printf("Content-type: text/html\n\n");
   if (msg!=NULL) {
      puts(msg);
      }
   fflush(stdout);
   return;
}
void set_cookie(char *name, char *cookie,char *extra)
{
if (extra==NULL) {
   printf("Set-cookie: %s=%s;\r\n",name,cookie);
   }
   else {
         printf("Set-cookie: %s=%s; %s\r\n",name,cookie,extra);
   }
   fflush(stdout);
}
WEBDATA *get_cookies()
{
   WEBDATA *wp;
   char *c;
   c=getenv("HTTP_COOKIE");
   if (c==NULL) return NULL;   /* There are no cookies */
   wp=str2webdata('=',';',c);
   if (wp==NULL) return NULL;
   return wp;
}
/*
 * convert a textarea to ascii
 */
char * textarea2asc(char *txt)
{
char *c,*c2,*ret,d[3];
int i;
c2=ret=(char *)xmalloc(strlen(txt)+1);
ret[0]='\0';
for(c=txt;*c;c++) {
   if(*c=='%') {
      for(i=0;i<2;i++) {
         ++c;
         if(*(c)=='\0') break;
         d[i]=*c;
      }
      if(*c=='\0') break;
      d[2]='\0';
      sscanf(d,"%x",&i);
      *c2=i;
      ++c2;
      continue;
   }
   if(*c=='+') *c2=' ';
     else *c2=*c;
   ++c2;
}
*c2='\0';
ret=xrealloc(ret,strlen(ret)+1);
return(ret);
}
