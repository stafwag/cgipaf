/*
 * CGIpaf acl functions
 *
 * author:  staf wagemakers <staf@patat.org>
 */

#include "acl.h"
#include "configfile.h"

#define  CFG_ACLORDER    	"AclOrder"
#define  CFG_ALLOWUSERS 	"AllowUsers"
#define  CFG_DENYUSERS  	"DenyUsers"
#define  CFG_ALLOWGRPS  	"AllowGroups"
#define  CFG_ALLOWUSERMAILDOMAIN  	"AllowUserMaildomains"
#define  CFG_DENYGRPS   	"DenyGroups"
#define  CFG_DENYUSERMAILDOMAIN   	"DenyUserMaildomains"
#define  ALLOW          	"allow"
#define  DENY           	"deny"


static int aclorder=1;          /* default = deny,allow */

/*
 * check if *p is a member of an Unix group
 * returns  1: is a member of grpname
 *          0: if not
 */
int memberofgrp (struct pw_info *p,char *grpname) {
    struct group *grp;
    char **gmem;
    
    grp=getgrnam(grpname);

    if(grp==NULL) return(0);
    if(grp->gr_gid==p->p->pw_gid) return(1);
    for (gmem=grp->gr_mem; *gmem != NULL; gmem++) 
	if(!strcmp(*gmem,p->p->pw_name)) return(1);
    return(0);
}

/*
 * Is *p is in the *users list?
 *
 * returns  1 : p is in the list
 *          0 : if p is not in the list
 */
int check_useracl(struct pw_info *p,char **users) {
    char **user;  
    char *name=p->p->pw_name;
    if(users==NULL) return(0);

    for (user=users; *user!=NULL;user++) {
	if(!strcmp(*user,"*"))  return(1);
	if(!strcmp(*user,name)) return(1);
    }

    return(0);
    
}
/*
 * Is p a member of the groups list
 *
 * returns  1 : p is a member
 *          0 : p isn't a member
 */
int check_grpacl(struct pw_info *p,char **groups) {
    char **group;
    char *name=p->p->pw_name;
    if(groups==NULL) return(0);

    for (group=groups;*group!=NULL;group++) {
	if(!strcmp(*group,"*")) return(1);
	if(memberofgrp(p,*group)==1) return(1);
    }

    return(0);

}

/*
 * Is *p is in the *users list?
 *
 * returns  1 : p is in the list
 *          0 : if p is not in the list
*/
int check_maildomainacl(char *maildomain,char **maildomains) {
    char **domain;
    if(maildomains==NULL) return(0);

    for (domain=maildomains; *domain!=NULL;domain++) {
        if(!strcmp(*domain,"*"))  return(1);
        if(!strcmp(*domain,maildomain)) return(1);
    }

    return(0);

}

/*
 * real aclorder function
 *
 * set *var to 0 -> ok
 *            -1 -> error 
 */ 
int real_aclorder(FILE *config_file,char *section_name,char *itemname,int *var) {
    char *c,*item,*part1,*part2;
    int i;

    item=get_sg_config(config_file,section_name,itemname);

    if (item==NULL) return(0);

    c=strchr(item,',');
    if(c==NULL) {
      *var=-1;
      return(-1);
    }
    
    i=c-item;
    part1=xmalloc(i+1);
    strncpy(part1,item,i);
    part1[i]='\0';
    c=mv_2_next(++c);
    part2=xmalloc(strlen(c)+1);
    strcpy(part2,c);
    cut_space(part2);
    if(!strcasecmp(part1,ALLOW)&&!strcasecmp(part2,DENY)) *var=0;
      else
      if(!strcasecmp(part1,DENY)&&!strcasecmp(part2,ALLOW)) *var=1;
      else
	      return(-1);
    xfree(part1);
    xfree(part2);
    return(0);
}

/*
 * Get GroupAclOrder out the configuration file
 *
 * set aclorder  to 0 if "allow,deny"
 *                  1 if "deny,allow"
 * returns aclorder
 */                     
int get_aclorder(FILE *config_file,char *section_name) {
    return(real_aclorder(config_file,section_name,CFG_ACLORDER,&aclorder));
}

/*
 * test the access control list
 *
 * returns 0  -> deny
 *         1  -> allowed
 */
int acl(FILE *config_file,char *section_name, struct pw_info *p,char *usermaildomain) {
    char **grpdeny=NULL,**grpallow=NULL;
    char **usrdeny=NULL,**usrallow=NULL;
    char **usermaildomain_deny=NULL,**usermaildomain_allow=NULL;
    int ret=0; 
    
    usrdeny=get_sg_config_array(config_file,section_name,CFG_DENYUSERS);
    usrallow=get_sg_config_array(config_file,section_name,CFG_ALLOWUSERS);
    
    grpdeny=get_sg_config_array(config_file,section_name,CFG_DENYGRPS);
    grpallow=get_sg_config_array(config_file,section_name,CFG_ALLOWGRPS);
   
    if(usermaildomain!=NULL) {

      usermaildomain_deny=get_sg_config_array(config_file,section_name,CFG_DENYUSERMAILDOMAIN);
      usermaildomain_allow=get_sg_config_array(config_file,section_name,CFG_ALLOWUSERMAILDOMAIN);

    }
 
    if(aclorder!=-1) {
    
      if(!aclorder) {
        ret=0;
        if(check_useracl(p,usrallow)) ret=1;
        if(check_grpacl(p,grpallow)) ret=1;
        if(usermaildomain!=NULL)
	  if(check_maildomainacl(usermaildomain,usermaildomain_allow)) ret=1;
        if(check_useracl(p,usrdeny)) ret=0;
        if(check_grpacl(p,grpdeny)) ret=0;
        if(usermaildomain!=NULL)
	  if(check_maildomainacl(usermaildomain,usermaildomain_deny)) ret=0;
      }
      else {
        ret=1;
        if(check_useracl(p,usrdeny)) ret=0;
        if(check_grpacl(p,grpdeny)) ret=0;
        if(usermaildomain!=NULL)
	  if(check_maildomainacl(usermaildomain,usermaildomain_deny)) ret=0;
        if(check_useracl(p,usrallow)) ret=1;
        if(check_grpacl(p,grpallow)) ret=1;
        if(usermaildomain!=NULL)
	  if(check_maildomainacl(usermaildomain,usermaildomain_allow)) ret=1;
      }
    
    }
    xfree(grpallow);
    xfree(usrallow);
    xfree(usermaildomain_allow);
    xfree(grpdeny);
    xfree(usrdeny);
    xfree(usermaildomain_deny);

    return(ret);
}
