#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#define THREADNUM 20
#define LINEBUF  1024
static int semId = 0;
void P(){
	struct sembuf buf;
	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = -1;
	while(semop(semId,&buf,1)<0){
		if(errno != EAGAIN && errno != EINTR){
			perror("semop")	;
			exit(1);
		}
	}

}

void V(){
	struct sembuf buf;
	buf.sem_flg = 0;
	buf.sem_num = 0;
	buf.sem_op = 1;
	while(semop(semId,&buf,1)<0){
		if(errno != EAGAIN && errno != EINTR){
			perror("semop")	;
			exit(1);
		}
	}


}

void *action(void * p){
	FILE * fp;
	fp = fopen("/tmp/add_log","r+");
	P();
	char  readstr[LINEBUF];
	fgets(readstr,LINEBUF, fp);
	fseek(fp,0, SEEK_SET);
	int i = atoi(readstr);
	fprintf(fp, "%d\n",i+1);
	fclose(fp);
	V();
	pthread_exit(NULL);
}
int main(int argc, char * argv[]){
	pthread_t pts[THREADNUM];
	semId = semget(IPC_PRIVATE,1,0600);
	int semctlId = semctl(semId, 0, SETVAL, 1);
	if( semctlId <  0){
		perror("semctl()");
		exit(1);	
	}
	for(int i=0; i< THREADNUM; i++){
		pthread_create(pts+ i,NULL,action,NULL);
	}
	for(int i=0; i< THREADNUM; i++){
		pthread_join(pts[i], NULL);	
	}	
	int remResult = semctl(semId, 0, IPC_RMID);
	exit(0);	
}
