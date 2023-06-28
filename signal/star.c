#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main(int argc , char * argv[]){
	sigset_t *  sig;
	sigset_t * old;
	sigemptyset(sig);
	sigaddset(sig,SIGINT );
	sigprocmask(SIG_UNBLOCK,sig, old);
	for(int i =0 ; i< 1000; i++){
		sigprocmask(SIG_BLOCK, sig ,NULL);
		for (int j= 0; j<5 ; j++){
			write(1,"*", 1);
			sleep(1);
		}
		write(1,"\n",1);
		sigprocmask(SIG_BLOCK, old , sig);
		pause();
	}
	return 1;
}
