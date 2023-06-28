#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main (int argc , char * argv[]){
	int64_t count = 0;
	time_t endtime = time(NULL) + 5;	
	while(time(NULL) <  endtime){
		count++;
	}
	printf("the result is %lld",count);
	exit(0);
}
