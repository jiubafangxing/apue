#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define START 30000010
#define END 30000410
#define ALL_THREADNUM END - START
typedef struct numstruct {
	int num;
} numstruct;


void * pnumaction(void * p);
int main(int argc, char * argv[]){
	void * returnp;
	pthread_t * pts[ALL_THREADNUM];

	for(int i = START; i<= END; i++){
		struct numstruct * nnumstructp = malloc(sizeof(struct numstruct *));
		nnumstructp->num = i;
		pthread_create(pts+(i-START) ,NULL, pnumaction,nnumstructp );
	}
	for (int j =0; j< ALL_THREADNUM; j++){
		pthread_join(*pts[j],&returnp);
		free(returnp);
	}
	exit(0);
}

void * pnumaction(void * p){
	
	int i,j,mark;
	struct numstruct * nums =(struct numstruct *) p;
	i = 	nums->num;
	mark  = 1;
	for(j=2; j<i/2 ; j++){
		if(i % j == 0){
			mark = 0;
			break;
		}
	}
	if(mark){
		printf("%d is a primer\n",i);
	}
	pthread_exit(p);
}
