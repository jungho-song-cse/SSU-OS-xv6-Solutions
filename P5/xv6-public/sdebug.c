#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5
#define PRINT_CYCLE 100000000
#define TOTAL_COUNTER 500000000

void sdebug_func(void){
	int n,pid;
	long long count; //variable for count cycles(p3)
	int prevTick;
	int currTick;

	printf(1,"start sdebug command\n");

	count=0;
	for(n=0; n<PNUM; n++){
		pid = fork();
		if(pid<0)
			break;
		if(pid==0){
			prevTick=uptime(); //save current uptime tick(p3)
		
			//set weight by systemcall weightset(p3)	
			if(weightset(n+1)==-1){
				printf(2,"Error, use weight bigger than 0!\n");
				break;
				exit();
			}
	
			for(;;){
				//tick print and stop each processes(p3)	
				if(count==PRINT_CYCLE){
					currTick = uptime()-prevTick; //save current uptime til this process information printed(p3)
					printProcInfo(currTick*10); //print current process information by system call(p3)
				}
				if(count>=TOTAL_COUNTER){ 
					printTerminated(); //print that current process terminated by system call(p3)
					exit(); //exit current process(p3)
				}
				count++; //count cycles(p3)
			}
			exit();
		}
	}

	//waits to make clear child processes. copied from forktest(p3)
	for(; n > 0; n--){
		if(wait() < 0){
			printf(1,"wait stopped early\n");
			exit();
		}
	}

	if(wait() != -1){
		printf(1,"wait got too many\n");
		exit();
	}


	printf(1,"end of sdebug command\n");
	return;
}

int main(void){
	sdebug_func();
	exit();
}
