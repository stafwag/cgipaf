/*
 * acl functions
 *
 * author:  staf wagemakers <staf@patat.org>
 */

#include "acl.h"
#include "configfile.h"

#define  USERACLORDER   "UserAclOrder"
#define  GRPACLORDER    "GroupAclOrder"
#define  USERALLOW	"UserAllow"
#define  USERDENY	"UserDeny"
#define  GRPALLOW	"GroupAllow"
#define  GRPDENY	"GroupDeny"
#define  ALLOW		"allow"
#define  DENY		"deny"
#define  DENYALLUSERS   "denyallusers"
#define  DENYALLGRPS    "denyallgroups"
#define  DENYALL	"DenyAll"

static int useraclorder=0;
static int grpaclorder=0;
static int denyallusers=0;
static int denyallgrps=0;


void set_denyallusers(int i) {
    denyallusers=i;
}

void set_denyallgrps(int i) {
    denyallgrps=i;
}

void set_denyall(int i) {
    denyallusers=denyallgrps=i;
}

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

int check_useracl(struct pw_info *p,char **users) {
    char **user;  
    char *name=p->p->pw_name;
    if(users==NULL) return(0);

    for (user=users; *user!=NULL;user++)
	if(!strcmp(*user,name)) return(1);

    return(0);
    
}

int check_grpacl(struct pw_info *p,char **groups) {
    char **group;
    char *name=p->p->pw_name;
    if(groups==NULL) return(0);

    for (group=groups;*group!=NULL;group++)
	if(memberofgrp(p,*group)==1) return(1);

    return(0);

}
int get_denyallusers(FILE *config_file,char *section_name) {
    char *cp;
    
    cp=get_sg_config(config_file,section_name,DENYALLUSERS);
    
    if(cp==NULL) return(denyallusers);
    if(!is_var_yes(cp)) denyallusers=0;
      else denyallusers=1;
    free(cp);
    return(denyallusers);
}


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

int get_useraclorder(FILE *config_file,char *section_name) {
    return(real_aclorder(config_file,section_name,USERACLORDER,&useraclorder));
}
int get_groupaclorder(FILE *config_file,char *section_name) {
    return(real_aclorder(config_file,section_name,GRPACLORDER,&grpaclorder));
}

int user_acl (FILE *config_file,char *section_name, struct pw_info *p) {
    char **deny,**allow;
    int ret=0; 

    deny=get_sg_config_array(config_file,section_name,USERDENY);
    allow=get_sg_config_array(config_file,section_name,USERALLOW);
    
    if(!useraclorder) {
      ret=check_useracl(p,allow)||(!check_useracl(p,deny)&&!denyallusers);
    }
    else {
      ret=!check_useracl(p,deny)&&!denyallusers&&check_useracl(p,allow);
    }

    free(allow);
    free(deny);

    return(ret);

}

int get_denyallgroups(FILE *config_file,char *section_name) {
    char *cp;
    
    cp=get_sg_config(config_file,section_name,DENYALLGRPS);
    
    if(cp==NULL) return(-1);
    if(!is_var_yes(cp)) denyallgrps=0;
      else denyallgrps=1;
    free(cp);
    return(denyallgrps);
}

int get_denyall(FILE *config_file,char *section_name) {
    char *cp;

    cp=get_sg_config(config_file,section_name,DENYALL);

    if(cp==NULL) return(-1);
    if(!is_var_yes(cp)) denyallgrps=denyallusers=0;
    else 
      denyallgrps=denyallusers=1;
    free(cp);
    return(denyallusers);
}

int group_acl (FILE *config_file,char *section_name, struct pw_info *p) {
    char **deny,**allow;
    int ret=0; 

    deny=get_sg_config_array(config_file,section_name,GRPDENY);
    allow=get_sg_config_array(config_file,section_name,GRPALLOW);
    
    if(!grpaclorder) {
      ret=check_grpacl(p,allow)||(!check_grpacl(p,deny)&&!denyallgrps);
    }
    else {
      ret=!check_grpacl(p,deny)&&!denyallusers&&check_grpacl(p,allow);
    }

    free(allow);
    free(deny);

    return(ret);
}
