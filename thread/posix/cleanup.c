#include <stdio.h>
#include <pthread.h>
static void *clean(int  p){
	fprintf(stdout, "clean thread %d\n",p);
	return &p;
}

static void  *my_thread(void *  argv){
	fprintf(stdout, "the thread is running\n");
	pthread_cleanup_push(clean,1);
	pthread_cleanup_pop(1);


	pthread_exit(NULL);
}
int main(int argc , char * argv[]){
	pthread_t p;
	pthread_create(&p,NULL,my_thread,NULL );
	return 0;
}

