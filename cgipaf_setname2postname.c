   /* set name to the loginname send by viewmailcfg.cgi */
   
   name=NULL;
   if ((cp=get_postitem(data,LOGIN))!=NULL) {
      name=textarea2asc(cp);
      options[0][1]=xmalloc(strlen(name)+1);
      strcpy(options[0][1],name);
      xfree(cp);
   }

