/*
 * accessdb.c                                       (c) 2001 Staf Wagemakers
 */
#include "accessdb.h"
/*
 * returns the status of a user
 * 0   -> ok
 * !0  -> have to wait return value (s) to wait
 * -1  -> failed
 */
int get_access_status (char *accessdb, char *loginname, int max_invalid, int delay) {
   DBM *db;
   datum data;
   datum key;
   time_t t;
   struct access_data access;
   db=dbm_open(accessdb, O_RDONLY, 0666);
   if(!db) return -1;
   key.dptr=(void *)loginname;
   key.dsize=strlen(loginname);
   data=dbm_fetch(db,key);
   if(!data.dptr) return(0);
   memcpy(&access,data.dptr,data.dsize);
   dbm_close(db);
   if (access.status==0) return 0;
   if (access.status<max_invalid) return 0;
   time(&t);
   if(t-access.ti>delay) return 0;
   return(delay-(t-access.ti));
}
/*
 * test_access_cookie
 * 
 * returns       -1:       error
 *                0:       not valid
 *                1:       ok
 */
int cmp_access_cookie(char *accessdb, char *loginname, char *cookie,time_t timeout)
{
   DBM *db;
   datum key;
   datum data;
   time_t t;
   struct access_data access;
   db=dbm_open(accessdb, O_RDONLY, 0666);
   if(!db) return -1;
   key.dptr=(void *)loginname;
   key.dsize=strlen(loginname);
   data=dbm_fetch(db,key);
   dbm_close(db);
   if(!data.dptr) return(0);
   memcpy(&access,data.dptr,data.dsize);
   if(strcmp(access.cookie,cookie)) {
      return(0);
   }
   time(&t);
   if(t-access.ti>timeout) {
      puts("debug: real timeout");
      return(0);
   }
   return(1);
}
/*
 * save the user status in *accessdb
 * 
 * return 0  -> ok
 *        -1 -> failed to update accessdb
 */
int save_access_status (char *accessdb, char *loginname, int status, int delay,char *cookie) {
   char txt_nul[]="0";
   DBM *db;
   datum key;
   datum data;
   time_t t;
   struct access_data access;
   if (cookie==NULL) {
      cookie=txt_nul;
      }
   db=dbm_open(accessdb, O_RDWR | O_CREAT, 0666);
   if (!db) return(-1);               /* failed to create db */
   key.dptr=(void *)loginname;
   key.dsize=strlen(loginname);
   time(&t);
   access.status=1;
   if (status) 
     {
         data=dbm_fetch(db,key);
         if (data.dptr) 
	   {
            memcpy(&access,data.dptr,data.dsize);
	    if (access.status && (t-access.ti>delay))
	      {
		access.status=0;
	      }
            ++access.status;
           }
      }
   else access.status=0;
   access.ti=t;
   memset(access.cookie,'\0',sizeof(access.cookie));
   strncpy(access.cookie,cookie,59);
   data.dptr=(void *)&access;
   data.dsize=sizeof(access);
   if (dbm_store(db,key,data,DBM_REPLACE)!=0) 
     {
       dbm_close(db);     
       return(-1); /* failed to store user data */
     }
   dbm_close(db);
   return(0);
}
