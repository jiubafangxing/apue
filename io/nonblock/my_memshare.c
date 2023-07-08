#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){
	char * str = 	mmap(NULL,1024,PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_SHARED,-1, 0);
	int pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}
	//child
	if(pid == 0){
		char * mystr = "hello";
		int mystrlen = strlen(mystr);
		memcpy(str, "hello",mystrlen);
		munmap(str,1024);
		exit(0);
	}else{
		wait(NULL);
		printf(str);
		munmap(str,1024);
		exit(0);

	}
}
