#include "types.h"
#include "stat.h"
#include "user.h"

//p2
int main(int argc, char *argv[]){
	int pid, wpid;
	if(argc<=2){ //if user input is less than two commands, inform usage
		printf(2,"usage: ssu_trace [mask(1 or more)] [command]\n");
		exit();
	}	

	if(atoi(argv[1])==0){ //if inputed mask is not a number, inform usage
		printf(2,"usage: ssu_trace [mask(1 or more)] [command]\n");
		exit();
	}

	trace(atoi(argv[1])); //set mask on current process to trace process
	
	//fork, and then exec inputed command, so make safe exec.(p2) 
	pid = fork(); //during fork, mask will be copied to child process(p2)
	if(pid < 0){
	printf(1, "ssu_trace: fork failed\n");
	exit();
	}
	if(pid == 0){
	exec(argv[2],argv+2);
	printf(1, "ssu_trace: exec %s failed\n",argv[2]);
	exit();
	}
	while((wpid=wait()) >= 0 && wpid != pid)
	printf(1, "zombie!\n");
	
	exit();	
}
