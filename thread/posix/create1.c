#include <stdio.h>
#include <pthread.h>

static void  *my_thread(void *  argv){
	fprintf(stdout, "the thread is running\n");
	return NULL;
}
int main(int argc , char * argv[]){
	pthread_t p;
	pthread_create(&p,NULL,my_thread,NULL );
	return 0;
}

