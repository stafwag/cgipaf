#include <stdio.h>
#include <string.h>
#include "pass.h"
#include "md5crypt.h"
int main (int argc, char **argv) {
char *name, *pass, * new_password, *encrypt_password;
struct pw_info *pw;
	if (argc!=4) {
		fprintf(stderr,"usage: %s name password newpassword\n",argv[0]);
	        exit(1);
	}
name=argv[1];
pass=argv[2];
new_password=argv[3];

printf("name: %s , pass: %s new_password : &s\n",name,pass,new_password);

pw=get_pw(name);

if (pw==NULL) {
	fprintf(stderr,"user not found\n");
	exit(1);
}
if ((ckpw(pw,pass))!=PASS_SUCCESS) {
	fprintf(stderr,"invalid password\n");
	exit(1);
}
printf("Auth OK\n");
encrypt_password=libshadow_md5_crypt(new_password,md5_seed());
printf ("md5 password: %s\n",encrypt_password);
chpw(pw,new_password);

}
