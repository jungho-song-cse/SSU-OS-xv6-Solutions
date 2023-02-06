#include "types.h"
#include "stat.h"
#include "user.h"

char buf[512];
int lines;

	void
cat(int fd)
{
	int n;
	int tempLines;

	tempLines = lines;

	if(tempLines==0){
		while((n = read(fd, buf, sizeof(buf))) > 0) {
			if (write(1, buf, n) != n) {
				printf(1, "cat: write error\n");
				exit();
			}
		}
	}
	else{
		while((n = read(fd, buf, 1))>0){
			if(buf[0]=='\n'){
				tempLines--;
			}
			if (write(1, buf, n) != n) {
				printf(1, "cat: write error\n");
				exit();
			}
			if(tempLines==0){
				break;
			}
		}
		if(tempLines>0){
			printf(2, "\n\nWARNING MESSAGE: Printed every lines, you've typed more than exsisting lines!!\n\n");
			exit();
		}
	}
	if(n < 0){
		printf(1, "cat: read error\n");
		exit();
	}

}

	int
main(int argc, char *argv[])
{
	int fd, i;

	if(argc <= 1){
		cat(0);
		exit();
	}


	lines = atoi(argv[1]);

	if(lines<=0){
		printf(2, "ERROR!!Please enter more than 1 after hcat!!\n");
		exit();
	}

	for(i = 2; i < argc; i++){
		if((fd = open(argv[i], 0)) < 0){
			printf(1, "cat: cannot open %s\n", argv[i]);
			exit();
		}
		cat(fd);
		close(fd);
	}
	exit();
}
