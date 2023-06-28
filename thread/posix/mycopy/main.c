#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 1024
enum {
 STAT_R = 1,
 STAT_W,
 STAT_EX,
 STAT_T
};
struct fsm_st{
   int status;
   int source;
   int dest;
   ssize_t bytes_read;
   char buf[BUFSIZE];
} fsm_st;
//状态机
void drive(struct fsm_st * st){
	switch(st->status){
		case STAT_R:
			st -> bytes_read = 	read(st->source, st->buf,BUFSIZE );	
			if(st->bytes_read  == 0){
				st -> status = STAT_T;
			}else if(st -> bytes_read < 0){
				if(errno == EAGAIN){
					st -> status = STAT_R;
				}else{
				
					st -> status = STAT_EX;
				}	
			}else{
				st -> status = STAT_W;
			}
			break;
		default:
			break;
	}
}
void relayer(int fd1 , int fd2 ){
	int save1 ,save2;
	//set io non block
	save1 = fcntl(fd1,F_GETFD);
	fcntl(fd1,save1|O_NONBLOCK);
	save2= fcntl(fd2,F_GETFD);
	fcntl(fd2,save2|O_NONBLOCK);

	struct fsm_st st1;
	st1.source= fd1;
	st1.dest = fd2;
	st1.status = STAT_R;
	struct fsm_st st2;
	st2.source= fd2;
	st2.dest = fd1;
	st2.status = STAT_R;
	drive(&st1);
	drive(&st2);
	// rollback old status
	fcntl(fd1,save1);
	fcntl(fd2,save2);
}

int main(int argc, char * argv[]){
	int fd1 = open(TTY1,O_RDWR);
	int fd2 = open(TTY2,O_RDWR);
	relayer(fd1,fd2);
	close(fd1);
	close(fd2);
	exit(0);
}



