/*
 * passpam.c
 * (GPL) 2000 - 02 Belgium                   http://stafwag.home7.dk3.com
 * Staf Wagemakers			      staf.wagemakers@advalvas.be
 */

#include "pass.h"

static char *oldpw;			    /* The old password */
static char *newpw;		            /* The new password */
static char *pam_msg=NULL;                  /* pam error message */
static int pwstate;			    /* current pam conversation
					       function state */

static int chauth_flag=PAM_CHANGE_EXPIRED_AUTHTOK;

char passwd_service[]="passwd";
int  set_pam_chauth_flag (int flag) {
     chauth_flag=flag;
}

char * set_pam_service(char *s)
{
    static char *pam_service=passwd_service;
    if (s!=NULL) pam_service=s;
    return pam_service;
}
char *last_pam_msg()
{
return(pam_msg);   
}

int chpasswd(num_msg, msg, resp, appdata_ptr)
    int          num_msg;
    const struct pam_message **msg;
    struct       pam_response **resp;
    void         *appdata_ptr;
{
    int i;
    struct pam_response *rp = xmalloc(sizeof(struct pam_response) * num_msg);
    if (rp==NULL) return(PAM_CONV_ERR);
    if (num_msg<=0) return(PAM_CONV_ERR);
    for (i=0; i<num_msg; i++) {
      if(msg[i]->msg_style==PAM_PROMPT_ECHO_OFF) {
	rp[i].resp_retcode=PAM_SUCCESS;
	if (pwstate==0) {
	   rp[i].resp=(char *)strdup(oldpw);
	   pwstate=1;
	}
	else {
	   rp[i].resp=(char *)strdup(newpw);
	}
	continue;
      }
      if((msg[i]->msg_style==PAM_TEXT_INFO)||(msg[i]->msg_style==PAM_ERROR_MSG)) {
	if(pam_msg!=NULL) free(pam_msg);
	pam_msg=xmalloc(strlen(msg[i]->msg)+1);
	strcpy(pam_msg,msg[i]->msg);
	rp[i].resp_retcode = PAM_SUCCESS;
	rp[i].resp = NULL;
	continue;
      }
      free (rp);
      return(PAM_CONV_ERR);
 }
  *resp = rp;
  return PAM_SUCCESS;
}

static struct pam_conv conv = {
	chpasswd, 
	NULL
};
/* ---------------------------------------------- */
/* reads the passwd info out /etc/passwd and      */
/* /etc/shadow                                    */
/*                                                */
/* name = loginname                               */
/* returns         passwd info                    */
/*                 0 = error                      */
/* ---------------------------------------------- */
struct pw_info * get_pw(char *name)
{
struct pw_info *pw;
/*
 * init pw
 */

pw=(struct pw_info *) xmalloc(sizeof(struct pw_info));
pw->p=NULL;
pw->sp=NULL;
pw->pamh=0;

/*
 * pwstate = authenticate
 */

pwstate=0;

/*
 * copy the user's /etc/passwd entry to pw->p and /etc/shadow to pw->sp
 * if the user doesn't exsists return NULL
 */

if(!(pw->p=getpwnam(name))) return(NULL);
if (!strcmp(pw->p->pw_passwd,"x")) {
#ifdef HAVE_SHADOW_H
   if(!(pw->sp=getspnam(name))) return(NULL);
#else
   /* 
    * shadow.h is unavailable, unable to copy useful shadow info in pw 
    */
   
   pw->sp=NULL;
#endif
   }

/*
 * start pam authentication
 */

if(pam_start(set_pam_service(NULL),name, &conv, &pw->pamh)!=PAM_SUCCESS) return(NULL);

/* 
 * the user exists and there is no pam error -> return the uses's info
 */

return(pw);
}
/* ---------------------------------------------- */
/* test a passwd                                  */
/* *p = passwd info                               */
/* *pass = password                               */
/* returns:        PAM_SUCCESS = ok               */
/*                 OTHER = error                  */
/* ---------------------------------------------- */
int ckpw(struct pw_info *pw,char *pass)
{
int ret;
pwstate=0;
oldpw = pass;
if((ret=pam_authenticate(pw->pamh,0))!=PAM_SUCCESS) {
   pam_end(pw->pamh,ret);
   return(ret);
}
if((ret=pam_acct_mgmt(pw->pamh,0))!=PAM_SUCCESS) {
   pam_end(pw->pamh,ret);
    return(ret);
}
return(ret);
}
int chpw(struct pw_info *pw,char *pass)
{
int ret;
pwstate=0;
newpw=pass;
ret=pam_chauthtok(pw->pamh,chauth_flag); 
return(ret);
}
