#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char userID[16][32];
char pwdID[16][32];

void get_user_list(){
	int fd, i, j, n;
	char buf[1];

	if((fd = open("list.txt", O_RDONLY))<0){
		printf(2, "file open ERROR! check list.txt file\n");
		exit();
	}

	for(i=0; i<16; i++){
		for(j=0; j<32; j++){
			if((n=read(fd, buf, 1))<0){
				printf(2, "read ERROR!\n");
				exit();
			}
			userID[i][j] = buf[0];
			if(userID[i][j]==' '){
				userID[i][j]='\n';
				break;
			}

		}

		for(;;){
			if((n=read(fd, buf, 1))<0){
				printf(2, "read ERROR!\n");
				exit();
			}
			if(buf[0]!=' '){
				pwdID[i][0] = buf[0];
				break;
			}
		}

		for(j=1; j<32; j++){
			if((n=read(fd, buf, 1))<0){
				printf(2, "read ERROR!\n");
				exit();
			}

			pwdID[i][j] = buf[0];
			if(pwdID[i][j]=='\n'||pwdID[i][j]==' '){
				pwdID[i][j]='\n';
				break;
			}

		}
	}
	if(n<0){
		printf(2,"read ERROR!\n");
		exit();
	}
}

int check_idpw(){
	char usernameInput[100];
	char passwordInput[100];

	static int count=0;
	if(count==13){
		printf(2,"Too much input.. Reinitializing shell..\n");
		exit();
	}
	int i;

	printf(1,"Username: ");
	gets(usernameInput, 31);

	for(i=0; i<16; i++){
		if(strcmp(usernameInput, userID[i])==0){
			break;
		}
	}

	if(i==16){
		printf(2,"##There is no Such name You've Entered!!!##\n\n");
		count++;
		check_idpw();
		return -1;
	}
	else{
		printf(1,"Password: ");
		gets(passwordInput, 31);

		if(strcmp(passwordInput, pwdID[i])!=0){
			printf(2,"Username / Password does not MATCH!!!\n\n");
			count++;
			check_idpw();
			return -1;
		}
	}

	return i;
}

int main(int argc, char *argv[]){
	int pid, wpid;
	get_user_list();

	check_idpw();

	for(;;){
		pid = fork();
		if(pid < 0){
			printf(1, "init: fork failed\n");
			exit();
		}
		if(pid == 0){
			exec("sh", argv);
			printf(1, "init: exec sh failed\n");
			exit();
		}
		while((wpid=wait()) >= 0 && wpid != pid)
			printf(1, "zombie!\n");
	}

	return 0;
}
