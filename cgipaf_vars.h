int    i,brol;
int    max_invalid=3;
int    invalid_timeout=600;
int    enable_cracklib=0;
struct pw_info *pw;
WEBDATA *data;
char   *name,*pass;
FILE   *config_file;
char   *doc_root;
char   txt_name[]="name";
char   txt_NULL[]="\0";
char   txt_max_invalid[]="max_invalid";
char   txt_invalid_timeout[]="invalid_timeout";
char   txt_invalid_wait[]="invalid_wait";
char   txt_too_much[]="<CENTER><H1><B><I><BR>Too much data!<BR></I></B></H1></CENTER>";
char   *cookie=NULL;
int    cookie_timeout=300;
char   max_invalid_txt[80]="3";
char   invalid_timeout_txt[80]="600";
char   invalid_wait_txt[80]="0";
char   *accessdb=NULL; 
char   *cp;
char   *newpass1,*newpass2;
char   txt_max_length[]="max_length";
char   txt_min_length[]="min_length";
char   max_length_txt[80]="8";
char   min_length_txt[80]="6";
char   txt_forward[]="forward";
char   forward[10]="no";
char   txt_not_forward[]="not_forward";
char   not_forward[10]="yes";
char   txt_forward_to[]="forward_to";
char   *forward_to;
char   txt_keep_msg[]="keep_msg";
char   keep_msg[10]="no";
char   txt_not_keep_msg[]="not_keep_msg";
char   not_keep_msg[10]="yes";
char   txt_autoreply[]="autoreply";
char   autoreply[10]="no";
char   txt_not_autoreply[]="not_autoreply";
char   not_autoreply[10]="yes";
char   txt_autoreply_msg[]="autoreply_msg";
char   txt_cookietimeout[]="cookie_timeout";
char   cookie_timeout_txt[80]="300";
char   *autoreply_msg=txt_NULL;
char   txt_yes[]="yes";
char   txt_no[]="no";
char   txt_password[]="password";
char   txt_passwd[]="passwd";
char   txt_crackliberror[]="cracklib_error";
char   txt_pamerrormsg[]="pam_error";
char   *pam_servicename=txt_passwd;
int    max_length, min_length;
char   *msg_success[]={"msg_success","msg_updated","msg_changed",NULL};
char   *options[][2]={
       txt_name,txt_NULL,                                /* 0 */
       txt_max_length,max_length_txt,                    /* 1 */
       txt_min_length,min_length_txt,                    /* 2 */
       txt_max_invalid,max_invalid_txt,                  /* 3 */
       txt_invalid_timeout,max_invalid_txt,              /* 4 */
       txt_invalid_wait,invalid_wait_txt,                /* 5 */
       txt_forward_to,txt_NULL,                          /* 6 */
       txt_forward,forward,                              /* 7 */
       txt_not_forward,not_forward,                      /* 8 */
       txt_keep_msg,keep_msg,                            /* 9 */
       txt_not_keep_msg,not_keep_msg,                    /* 10 */
       txt_autoreply,autoreply,                          /* 11 */
       txt_not_autoreply,not_autoreply,                  /* 12 */
       txt_autoreply_msg,txt_NULL,                       /* 13 */
       txt_cookietimeout,cookie_timeout_txt,             /* 14 */
       txt_password,txt_NULL,                            /* 15 */
       txt_crackliberror,txt_NULL,			 /* 16 */
       txt_pamerrormsg,txt_NULL,			 /* 17 */
       NULL,NULL};
