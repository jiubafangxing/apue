#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <pthread.h>
#include <unistd.h>

#define THREADNUM 12
#define LINEBUF  1024

void *action(void * p){
	FILE * fp;
	fp = fopen("/tmp/add_log","r+");
	char  readstr[LINEBUF];
	fgets(readstr,LINEBUF, fp);
	fseek(fp,0, SEEK_SET);
	int i = atoi(readstr);
	fprintf(fp, "%d\n",i+1);
	fclose(fp);
	pthread_exit(NULL);
}
int main(int argc, char * argv[]){
	pthread_t pts[THREADNUM];
	for(int i=0; i< THREADNUM; i++){
		pthread_create(pts+ i,NULL,action,NULL);
	}
	for(int i=0; i< THREADNUM; i++){
		pthread_join(pts[i], NULL);	
	}	
	exit(0);	
}
