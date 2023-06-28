#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define START 30000010
#define END 30000410
#define ALL_THREADNUM 4


static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;

int num = 0;
void * pnumaction(void * p);
int main(int argc, char * argv[]){
	void * returnp;
	pthread_t * pts[ALL_THREADNUM];
	for(int i = 0; i< ALL_THREADNUM; i++){
		pthread_create(pts+i ,NULL, pnumaction,(void *)i);
	}
	for (int i = START; i<=  END;i++){
		pthread_mutex_lock(&mut_num);
		while(num != 0){
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}
		num = i;
		pthread_mutex_unlock(&mut_num);
	}
	while(num == 0){ 
		pthread_mutex_lock(&mut_num);
		num = -1;
		pthread_mutex_unlock(&mut_num);
	}
	for (int j =0; j< ALL_THREADNUM; j++){
		pthread_join(*pts[j],NULL);
	}
	pthread_mutex_destroy(&mut_num);
	exit(0);
}

void * pnumaction(void * p){
	
	int i,j,mark;		
	while(1){
		pthread_mutex_lock(&mut_num);
		while(num == 0){
			pthread_mutex_unlock(&mut_num);
			sched_yield();
			pthread_mutex_lock(&mut_num);
		}
		if(num == -1){
			pthread_mutex_unlock(&mut_num);
			break;
		}
		i = num;
		mark  = 1;
		for(j=2; j<i/2 ; j++){
			if(i % j == 0){
				mark = 0;
				break;
			}
		}
		if(mark){
				printf("%d is a primer thread is %d\n",num,(int)p);
		}

		num = 0;
		pthread_mutex_unlock(&mut_num);
	}
	pthread_exit(NULL);
}
