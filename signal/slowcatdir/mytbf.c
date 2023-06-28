#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "mytbf.h"

static struct mytbf_st * mytbfs[MYTBF_MAX];

static int moduleload_flag = 0;


typedef void (* signalhandler_t)(int);

static signalhandler_t handler ;
static int findFreePosition(){
	for(int i= 0; i< MYTBF_MAX; i++){
		if(mytbfs [i] == NULL){
			return i;
		}
	}
	return -1;
}
static int min (int  o1 , int o2){
	if(o1 >  o2){
		return o2;
	}else{
		return o1;
	}
}

static void  alarmhandler(){
	 int pos = 0;
	 for(;pos< MYTBF_MAX; pos++){
	 	struct mytbf_st * op = mytbfs[pos];
		if(NULL != op){
			op ->token += op->cps;

		}
	 }
	 alarm(1);
}
struct mytbf_st * mytbf_init(int cps, int burst){
	struct	mytbf_st * mytb;	
	mytb = malloc(sizeof(struct mytbf_st *));
	mytb -> cps = cps;
	mytb -> burst = burst;
	mytb -> token = 0;
	if(moduleload_flag  == 0){
		moduleload_flag = 1;
		alarm(1);
		handler = signal(SIGALRM,alarmhandler);
	}
	int freePosition = findFreePosition();
	if(freePosition < 0){
		return NULL;
	}
	mytb -> position = freePosition;
	mytbfs[freePosition] = mytb;
	return mytb;
}

int mytbf_fetchtoken(struct mytbf_st * mytbf_st_v , int size){
	int tokensize ;
	while((tokensize = min(size, mytbf_st_v -> token)) < 1){
		pause();
	}
	mytbf_st_v->token -=   tokensize ;
	return tokensize;
}

int mytbf_returntoken(struct mytbf_st * mytbf_st_v,int size ){
	int tokensize ;
	int returnPresize = mytbf_st_v -> token + size;
	int returnsize = 	min(returnPresize, mytbf_st_v -> burst);
	mytbf_st_v -> token = returnsize;
	return size;
}


int mytbf_desory(mytbf_st * mytbf_st_v){
	int positon = mytbf_st_v ->position;
	mytbfs[positon] = NULL;
	free(mytbf_st_v);	
	return 1;
}


void module_destory(void){
	int pos = 0; 
	for(;pos< MYTBF_MAX; pos++){
		free(mytbfs[pos]);
	}
	alarm(0);
	signal(SIGALRM, handler);
}
