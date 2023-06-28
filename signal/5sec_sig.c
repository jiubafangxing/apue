#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

static volatile int  loop =1;
static  void alarm_sig(int s ){
       loop = 0;	
}

int main (int argc , char * argv[]){
	int64_t count = 0;
	alarm(5);
	signal(SIGALRM, alarm_sig);	
	while(loop){
		count++;
	}
	printf("the result is %lld",count);
	exit(0);
}
