/* ---------------------------------------------------------------------- */
/* passpam.c						                  */
/*									  */
/* (GPL) 2000,2001 Belgium                     http://www.stafwag.f2s.com */
/* Staf Wagemakers                                        stafwag@f2s.com */
/* ---------------------------------------------------------------------- */
#include "pass.h"
char *oldpw;
char *newpw;
static int pwstate;
int chpasswd(num_msg, msg, resp, appdata_ptr)
    int         num_msg;
    const struct pam_message **msg;
    struct       pam_response **resp;
    void         *appdata_ptr;
{
    int i;
    struct pam_response *rp = xmalloc(sizeof(struct pam_response) * num_msg);
    if (rp==NULL) return(PAM_SESSION_ERR);
    if (!rp) return(PAM_CONV_ERR);
    if (num_msg<=0) return(PAM_CONV_ERR);
    for (i=0; i<num_msg; i++) {
	if(msg[i]->msg_style==PAM_ERROR_MSG) return(PAM_CONV_ERR);
	rp[i].resp_retcode = 0;
        if ((msg[i]->msg_style!=PAM_PROMPT_ECHO_OFF)&&(msg[i]->msg_style!=PAM_PROMPT_ECHO_ON)) return(PAM_CONV_ERR);
        if (pwstate) rp[i].resp=(char *) strdup(newpw);
            else rp[i].resp=(char *) strdup(oldpw);
   }
   *resp=rp;
   return PAM_SUCCESS;
}
struct pam_conv conv = {
       chpasswd,NULL
       };
/* ---------------------------------------------- */
/* reads the passwd info out /etc/passwd and      */
/* /etc/shadow					  */
/*                                                */
/* name = loginname				  */
/* returns         passwd info		          */
/*                 0 = error                      */
/* ---------------------------------------------- */
struct pw_info * get_pw(char *name)
{
struct pw_info *pw;
pw=(struct pw_info *) xmalloc(sizeof(struct pw_info));
pw->p=NULL;
pw->sp=NULL;
pw->pamh=0;
pwstate=0;
if(!(pw->p=getpwnam(name))) return(NULL);
if (!strcmp(pw->p->pw_passwd,"x")) {
   if(!(pw->sp=getspnam(name))) return(NULL);
   }
if(pam_start("passwd",name, &conv, &pw->pamh)!=PAM_SUCCESS) return(NULL);
return(pw);
}
/* ---------------------------------------------- */
/* test a passwd                                  */
/* *p = passwd info				  */
/* *pass = password				  */
/* returns:        0 = wrong passwd               */
/*                 1 = ok			  */
/* ---------------------------------------------- */
int ckpw(struct pw_info *pw,char *pass)
{
pwstate=0;
oldpw=pass;
if(pam_authenticate(pw->pamh,0)!=PAM_SUCCESS) return(-1);
return(0);
}
int chpw(struct pw_info *pw,char *pass)
{
pwstate=1;
newpw=pass;
if (pam_chauthtok(pw->pamh,0)!=PAM_SUCCESS) return(-13);
pam_end(pw->pamh,0);
return(0);
}
