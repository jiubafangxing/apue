#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "mytbf.h"


static int moduleload_flag = 0;
static pthread_t  settoken_thread;
static pthread_mutex_t mutex_t = PTHREAD_MUTEX_INITIALIZER;
static struct mytbf_st * mytbfs[MYTBF_MAX];
pthread_once_t once_control = PTHREAD_ONCE_INIT;

typedef void (* signalhandler_t)(int);
static int findFreePosition(){
	for(int i= 0; i< MYTBF_MAX; i++){
		if(mytbfs [i] == NULL){
			return i;
		}
	}
	return -1;
}
static signalhandler_t handler ;
static int min (int  o1 , int o2){
	if(o1 >  o2){
		return o2;
	}else{
		return o1;
	}
}

void * alarmhandler(void * nn){
	while(1){
		pthread_mutex_lock(&mutex_t);
		for(int i =0; i < MYTBF_MAX; i++){
			if(NULL != mytbfs[i]){

			pthread_mutex_lock(&mytbfs[i]->mymutex );
			struct	mytbf_st * mytbf;	
			mytbf = mytbfs[i];			
			int tosetTokenSize = mytbf->token + mytbf->cps;
			int realSetToken =  min(tosetTokenSize, mytbf->burst);
			mytbf->token = realSetToken;
			pthread_cond_broadcast(&mytbfs[i]->mycondition);
			pthread_mutex_unlock(&mytbfs[i]->mymutex );
			}
		}
		pthread_mutex_unlock(&mutex_t);
		sleep(1);
	}
}
void init_once(){
//	int err = 	pthread_create(settoken_thread,NULL,alarmhandler,NULL);
	int err = pthread_create(&settoken_thread,NULL,alarmhandler,NULL);
	if(err){
		fprintf(stderr,"create pthread err :%s\n",strerror(err));
		exit(1);
	}
	atexit(module_destory);
}
struct mytbf_st * mytbf_init(int cps, int burst){
	struct	mytbf_st * mytb;	
	
	mytb = malloc(sizeof(* mytb));
	mytb -> cps = cps;
	mytb -> burst = burst;
	mytb -> token = 0;
	
	pthread_mutex_init(&(mytb->mymutex),NULL);
	pthread_cond_init(&(mytb->mycondition),NULL);
        
	pthread_once(&once_control,init_once);
	pthread_mutex_lock(&mutex_t);
	
	int freePostion = findFreePosition();
	if(-1 == freePostion){
		pthread_mutex_unlock(&mutex_t);
		return NULL;
	}
	mytbfs [freePostion] = mytb;
	pthread_mutex_unlock(&mutex_t);
	return mytb;
}

int mytbf_fetchtoken(struct mytbf_st * mytbf_st_v , int size){
	
	int tokensize ;
	pthread_mutex_lock(&mytbf_st_v->mymutex);
	while((tokensize = min(size, mytbf_st_v -> token)) < 1){
		pthread_cond_wait(&(mytbf_st_v->mycondition),&(mytbf_st_v->mymutex));
	}
	mytbf_st_v->token -=   tokensize ;
	pthread_mutex_unlock(&mytbf_st_v->mymutex);
	return tokensize;
}

int mytbf_returntoken(struct mytbf_st * mytbf_st_v,int size ){
	int tokensize ;

	pthread_mutex_lock(&mytbf_st_v->mymutex);
	int returnPresize = mytbf_st_v -> token + size;
	int returnsize = 	min(returnPresize, mytbf_st_v -> burst);
	mytbf_st_v -> token = returnsize;
	pthread_cond_broadcast(&(mytbf_st_v->mycondition));	
	pthread_mutex_unlock(&mytbf_st_v->mymutex);
	return size;
}


int mytbf_desory(mytbf_st * mytbf_st_v){
	int positon = mytbf_st_v ->postion;
	pthread_mutex_destroy(&mytbf_st_v->mymutex);
	pthread_cond_destroy(&(mytbf_st_v->mycondition));
	free(mytbf_st_v);	
	return 1;
}


void module_destory(void){
	pthread_cancel(settoken_thread);
	pthread_join(settoken_thread,NULL);
	for(int i = 0; i< MYTBF_MAX; i++){
		if(mytbfs[i] != NULL){
			mytbf_desory(mytbfs[i]);
		}
	}
	
	pthread_mutex_destroy(&mutex_t);
}
