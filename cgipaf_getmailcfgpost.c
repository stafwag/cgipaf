   /* set forward and not_forward */
   
   c=get_postitem(data,FORWARD);
   yes_no(c,forward,not_forward,&forward_state);
   xfree(c);

   /* set forward_to */
   
   c=get_postitem(data,FORWARD_TO);
   
   if(c!=NULL) {
      char *c2;
      c2=textarea2asc(c);
      if(c2==NULL) {
	 print_txt_msg("out of memory");
	 write_log(LOG_USER,0,"out of memory");
	 exit(0);
      }
      xfree(c);
      forward_to=xmalloc(strlen(c2)+1);
      strcpy(forward_to,c2);
      xfree(c2);
   }

   /* set keep_msg */
   
   c=get_postitem(data,KEEP_MSG);
   yes_no(c,keep_msg,not_keep_msg,&keep_state);
   xfree(c);
   
   /* set autoreply */
   
   c=get_postitem(data,AUTOREPLY);
   yes_no(c,autoreply,not_autoreply,&autoreply_state);
   xfree(c);

   /* set autoreply_msg */
   
   c=get_postitem(data,AUTOREPLY_MSG);
   if(c!=NULL) {
      autoreply_msg=textarea2asc(c);
      xfree(c);
   }
   
   /* copy the data in the options array */
   
   options[6][1]=forward_to;
   options[13][1]=autoreply_msg;

