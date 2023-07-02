#include "relayer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define TTY3 "/dev/tty9"
#define TTY4 "/dev/tty10"
int main(int argc, char * argv[]){
	int fd1 = 	open(TTY1,O_RDWR);	
	if(fd1 < 0){
		printf("open err %d",fd1);
	}
	int fd2 = 	open(TTY2,O_RDWR);	
	printf("init 1\n");
	relayer_init(fd1, fd2);
//	int fd3 = 	open(TTY3,O_RDWR);	
//	int fd4 = 	open(TTY4,O_RDWR);	
//	printf("init 2\n");
//	relayer_init(fd3, fd4);
	while(1){
		pause();		
	}
	return 0;
}
