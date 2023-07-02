#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <err.h>
#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 24
#define RELAY_SIZE  5
static pthread_once_t once_block = PTHREAD_ONCE_INIT;
static pthread_mutex_t mutext = PTHREAD_MUTEX_INITIALIZER;
static pthread_t mytask;
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
	int position;
	char * errmsg;
	ssize_t bytes_read;
	char buf[BUFSIZE];
} fsm_st;
struct relayer_st{
	int fd1;
	int fd2;
	int save1;
	int save2;
	int task_state;
	struct fsm_st * st12;
	struct fsm_st * st21;
}relayer_st;


static struct relayer_st  *relayPool[RELAY_SIZE] ;
void drive(struct fsm_st * st);
static void *driveAction(void * param){
	while(1){
		pthread_mutex_lock(&mutext);
		for(int i =0; i<RELAY_SIZE; i++){
			struct relayer_st * st = relayPool[i];
			if(NULL != st   ){
					drive(st->st12);
					drive(st->st21);
			}
		}

		pthread_mutex_unlock(&mutext);
	}
	return NULL;
}
static void loadthread(void){
	printf("before create\n");
	pthread_t pt;
	int pid =  pthread_create(&pt, NULL, driveAction, NULL);
	printf("pid create result %d\n",pid);
}
static int findFreePosition(){
	for(int i= 0; i< RELAY_SIZE; i++){
		if( relayPool[i] == NULL){
			return i;
		}
	}
	return -1;
}
//状态机
void drive(struct fsm_st * st){
	int writed = 0;
	switch(st->status){
		case STAT_R:
			st -> bytes_read = 	read(st->source, st->buf,BUFSIZE );	
			if(st->bytes_read  == 0){
				st -> status = STAT_T;
			}else if(st -> bytes_read < 0){
				if(errno == EAGAIN){
					st -> status = STAT_R;
				}else{
					st -> errmsg = "read error";
					st -> status = STAT_EX;
				}	
			}else{
				st -> position = 0;
				st -> status = STAT_W;
			}
			break;
		case STAT_W:
		 	 writed = 	write(st->dest, st->buf + st->position,st->bytes_read);
			
				if(writed < 0){
					if(errno == EAGAIN){
						st -> status = STAT_W;
					}else{
						printf("error writeing\n ");
						st -> errmsg = "write error";
						st -> status = STAT_EX;
					}

				}else {
					st -> bytes_read  -= writed;
					st -> position += writed;
					if(st -> bytes_read == 0){
						st -> status = STAT_R;
					}else{
						st -> status = STAT_W;
					}
				}
			break;
		case STAT_EX:
			st -> status = STAT_T;
			perror(st->errmsg);
			break;
		case STAT_T:

			break;
		default:
			abort();
			break;
	}
}
int relayer_init(int fd1, int fd2){
	
	struct relayer_st * relayst; 
	relayst = malloc(sizeof(* relayst))	;
	relayst -> save1 = fcntl(fd1,F_GETFD);
	fcntl(fd1,relayst->save1|O_NONBLOCK);
	relayst -> save2= fcntl(fd2,F_GETFD);
	fcntl(fd2, relayst->save2|O_NONBLOCK);

	struct fsm_st st1;
	st1.source= fd1;
	st1.dest = fd2;
	st1.status = STAT_R;
	struct fsm_st st2;
	st2.source= fd2;
	st2.dest = fd1;
	st2.status = STAT_R;
	relayst->st12 = &st1;
	relayst->st21 = &st2;
	pthread_mutex_lock(&mutext);
	int free = findFreePosition();
	if(free < 0){
		return -ENOSPC;	
	}
	relayPool[free] = relayst;
	printf("set once%d\n",free 
			);
	pthread_mutex_unlock(&mutext);
	pthread_once(&once_block,loadthread);
	return free;
}




