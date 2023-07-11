#include "mypipe.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int min(int count1, int count2 ){
	if(count2 >count1){
		return count1;
	}else{
		return count2;
	}
}


//初始化锁
struct my_pipe_stat{
	int head;
	int tail;
	int pipe_size;
	char data[PIPESIZE];
	pthread_mutex_t mutex_t;
	pthread_cond_t cond_t;
};
my_pipe_stat *init(){
	struct my_pipe_stat * stat;
	stat = malloc(sizeof(stat));
	stat->head = 0;
	stat->tail= 0;
	stat->pipe_size = 0;
	pthread_mutex_init(&stat->mutex_t, NULL);
	pthread_cond_init(&stat->cond_t,NULL);
	return stat;
}
int next(int head, int size){
	return (head+1)%size;
}

int readOneByte(my_pipe_stat * stat, char * buf){
	
	struct my_pipe_stat * me = stat;
	if(me->pipe_size <= 0 ){
		return -1;
	}
	*buf = me->data[me->head];
	int size = next(me->head,me->pipe_size);
	me->pipe_size -= 1;
	return 1;
		
}
int read(my_pipe_stat * stat,void *buf, size_t readSize){	
	struct my_pipe_stat * me = stat;
	pthread_mutex_lock(&me->mutex_t);
	while(&me->pipe_size <= 0){
		pthread_cond_wait(&me->cond_t,&me->mutex_t);
	}
	int toRead = min(readSize,me->pipe_size);
	for(int i= 0; i< readSize; i++){
		readOneByte(buf, stat);	
	}
	pthread_mutex_unlock(&me->mutex_t);
	return 0;
}


int wirte(my_pipe_stat *,const void * buf,  int readSize){
	return 0;
}

void destory( my_pipe_stat * stat){
	struct my_pipe_stat * me = stat; 
	pthread_mutex_destroy(&me->mutex_t);
	pthread_cond_destroy(&me->cond_t);
	free(me);
	return ;
}
