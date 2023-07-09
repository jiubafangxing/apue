#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/wait.h>
#define PROCNUM 12
#define LINEBUF  1024

void action(void){
	FILE * fp;
	fp = fopen("/tmp/add_log","r+");
	char  readstr[LINEBUF];
	fgets(readstr,LINEBUF, fp);
	int fd = fileno(fp);
	lockf(fd,F_LOCK, 0 );
	fseek(fp,0, SEEK_SET);
	int i = atoi(readstr);
	fprintf(fp, "%d\n",i+1);
	sleep(1);
	fflush(fp);
	flock(fd,LOCK_UN);
	fclose(fp);
}
int main(int argc, char * argv[]){
	for(int i=0; i< PROCNUM; i++){
		int pid = fork();
		if(pid == 0){
			action();			
			exit(0);
		}else{
			wait(NULL);
		}
	}
	
			exit(0);	
}
