unsetenv("IFS");
set_memerr(out_of_memory);      /* set out of memory handler */

options=add_2_string_pair(options,txt_name,txt_NULL);				/* 0 */
options=add_2_string_pair(options,txt_max_length,max_length_txt);		/* 1 */
options=add_2_string_pair(options,txt_min_length,min_length_txt);              	/* 2 */
options=add_2_string_pair(options,txt_max_invalid,max_invalid_txt);            	/* 3 */
options=add_2_string_pair(options,txt_invalid_timeout,max_invalid_txt);        	/* 4 */
options=add_2_string_pair(options,txt_invalid_wait,invalid_wait_txt);          	/* 5 */
options=add_2_string_pair(options,txt_forward_to,txt_NULL);                    	/* 6 */
options=add_2_string_pair(options,txt_forward,forward);                        	/* 7 */
options=add_2_string_pair(options,txt_not_forward,not_forward);                	/* 8 */
options=add_2_string_pair(options,txt_keep_msg,keep_msg);                      	/* 9 */
options=add_2_string_pair(options,txt_not_keep_msg,not_keep_msg);              	/* 10 */
options=add_2_string_pair(options,txt_autoreply,autoreply);                    	/* 11 */
options=add_2_string_pair(options,txt_not_autoreply,not_autoreply);            	/* 12 */
options=add_2_string_pair(options,txt_autoreply_msg,txt_NULL);                 	/* 13 */
options=add_2_string_pair(options,txt_cookietimeout,cookie_timeout_txt);       	/* 14 */
options=add_2_string_pair(options,txt_password,txt_NULL);                      	/* 15 */
options=add_2_string_pair(options,txt_crackliberror,txt_NULL);			/* 16 */
options=add_2_string_pair(options,txt_pamerrormsg,txt_NULL);		       	/* 17 */
options=add_2_string_pair(options,txt_mailcfg_exitcode,txt_NULL);              	/* 18 */
options=add_2_string_pair(options,txt_homedir,txt_NULL);                       	/* 19 */
options=add_2_string_pair(options,txt_domain,txt_NULL);                        	/* 20 */
options=add_2_string_pair(options,txt_badpassword,txt_NULL);		       	/* 21 */
options=add_2_string_pair(options,txt_usermaildomain,txt_NULL);	       		/* 22 */

