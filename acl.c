/*
 * CGIpaf acl functions
 *
 * author:  staf wagemakers <staf@patat.org>
 */

#include "acl.h"
#include "configfile.h"

#define  CFG_USERACLORDER   	"UserAclOrder"
#define  CFG_GRPACLORDER    	"GroupAclOrder"
#define  CFG_ALLOWUSERS 	"AllowUsers"
#define  CFG_DENYUSERS  	"DenyUsers"
#define  CFG_ALLOWGRPS  	"AllowGroups"
#define  CFG_DENYGRPS   	"DenyGroups"
#define  ALLOW          	"allow"
#define  DENY           	"deny"


static int useraclorder=0;
static int grpaclorder=0;

/*
 * check if *p is a member of an Unix group
 * returns  1: is a member of grpname
 *          0: if not
 *         -1: error
 */
int memberofgrp (struct pw_info *p,char *grpname) {
    struct group *grp;
    char **gmem;
    
    grp=getgrnam(grpname);

    if(grp==NULL) return(-1);
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
 * real aclorder function
 *
 * set *var to 0 if "allow,deny"
 *             1 if "deny,allow"
 *            -1 if none of above
 * returns     *var
 */ 
int real_aclorder(FILE *config_file,char *section_name,char *itemname,int *var) {
    char *c,*item,*part1,*part2;
    int i;

    item=get_sg_config(config_file,section_name,itemname);

    if (item==NULL) {
	     *var=0;
	     return(*var);
	   }


    c=strchr(item,',');
    if(c==NULL) {
      *var=-1;
      return(*var);
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
	      *var=-1;
    free(part1);
    free(part2);
    return(*var);
}
/*
 * Get UserAclOrder out the configuration file
 *
 * set useraclorder to 0 if "allow,deny"
 *                     1 if "deny,allow"
 * returns useraclorder
 */                     
int get_useraclorder(FILE *config_file,char *section_name) {
    return(real_aclorder(config_file,section_name,CFG_USERACLORDER,&useraclorder));
}

/*
 * Get GroupAclOrder out the configuration file
 *
 * set grpaclorder  to 0 if "allow,deny"
 *                     1 if "deny,allow"
 * returns grpaclorder
 */                     
int get_groupaclorder(FILE *config_file,char *section_name) {
    return(real_aclorder(config_file,section_name,CFG_GRPACLORDER,&grpaclorder));
}

/*
 * test the user access control list
 *
 * returns 0  -> deny
 *         1  -> allowed
 */
int user_acl (FILE *config_file,char *section_name, struct pw_info *p) {
    char **deny,**allow;
    int ret=1; 

    if(useraclorder==-1)  return(0); 	/* deny on aclorder error */
    
    deny=get_sg_config_array(config_file,section_name,CFG_DENYUSERS);

    allow=get_sg_config_array(config_file,section_name,CFG_ALLOWUSERS);
    
    if((deny==NULL)&&(allow==NULL)&&!useraclorder) return(1);
    
    if(!useraclorder) {
      if(check_useracl(p,allow)) ret=1;
        else
         if(check_useracl(p,deny)) ret=0;
    }
    else {
      if(check_useracl(p,deny)) ret=0;
	else
         if(check_useracl(p,allow)) ret=1;
    }

    free(allow);
    free(deny);

    return(ret);

}

/*
 * test the group access control list
 *
 * returns 0  -> deny
 *         1  -> allowed
 */
int group_acl (FILE *config_file,char *section_name, struct pw_info *p) {
    char **deny,**allow;
    int ret=1; 

    if(grpaclorder==-1)  return(0);   /* deny on aclorder error */
    
    deny=get_sg_config_array(config_file,section_name,CFG_DENYGRPS);
    allow=get_sg_config_array(config_file,section_name,CFG_ALLOWGRPS);
    
    if((deny==NULL)&&(allow==NULL)&&!grpaclorder) return(1);
    
    if(!grpaclorder) {
      if(check_grpacl(p,allow)) ret=1;
        else
         if(check_grpacl(p,deny)) ret=0;
    }
    else {
      if(check_grpacl(p,deny)) ret=0;
	else
         if(check_grpacl(p,allow)) ret=1;
    }

    free(allow);
    free(deny);

    return(ret);
}
