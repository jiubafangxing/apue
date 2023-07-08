#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 1024
enum {
	STAT_R = 1,
	STAT_W,
	STAT_AUTO,
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

int max(int fd1, int fd2){
	if(fd1 > fd2){
		return fd1;
	}else{
		return fd2;
	}
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
void relayer(int fd1 , int fd2 ){
	int save1 ,save2;
	//set io non block
	save1 = fcntl(fd1,F_GETFD);
	fcntl(fd1,F_SETFL, save1|O_NONBLOCK);
	save2= fcntl(fd2,F_GETFD);
	fcntl(fd2,F_SETFL, save2|O_NONBLOCK);

	struct fsm_st st1;
	st1.source= fd1;
	st1.dest = fd2;
	st1.status = STAT_R;
	struct fsm_st st2;
	st2.source= fd2;
	st2.dest = fd1;
	st2.status = STAT_R;
	int epollfd = epoll_create(2);
	struct epoll_event ev;
	ev.data.fd = fd1;
	ev.events = 0;
	epoll_ctl(epollfd,EPOLL_CTL_ADD, fd1,&ev);
	ev.data.fd = fd2;
	ev.events = 0;
	epoll_ctl(epollfd,EPOLL_CTL_ADD, fd2,&ev);

	while(st1.status != STAT_T && st2.status != STAT_T){

		ev.events = 0;
		if(st1.status == STAT_R){
			ev.data.fd = fd1;
			ev.events |= EPOLLIN;
		}
		if(st2.status == STAT_W){
			ev.data.fd = fd1;
			ev.events |= EPOLLOUT;
		}
		epoll_ctl(epollfd, EPOLL_CTL_MOD, fd1, &ev);
		if(st2.status == STAT_R){
			ev.data.fd = fd2;
			ev.events |= EPOLLIN;
		}
		if(st1.status == STAT_W){
			ev.data.fd = fd2;
			ev.events |= EPOLLOUT;
		}
		epoll_ctl(epollfd, EPOLL_CTL_MOD, fd2, &ev);
		if(st1.status < STAT_AUTO || st2.status <STAT_AUTO){
			//if(select(max(fd1, fd2)+1, &rfds,&wfds, NULL,NULL) <0){
			while(epoll_wait(epollfd, &ev,1, -1) < 0)
				if(errno == EINTR ){
					continue;
				}else{
					perror("select ()");
					exit(1);
				}
		}
		if((ev.data.fd == fd1 && (ev.events & EPOLLIN ))|| (ev.data.fd == fd2 && (ev.events & EPOLLOUT)) || st1.status > STAT_AUTO){
			drive(&st1);
		}
		if((ev.data.fd == fd2 && (ev.events & EPOLLIN) )|| (ev.data.fd == fd1 && (ev.events & EPOLLOUT)) || st2.status > STAT_AUTO){

			drive(&st2);
		}
	}

	// rollback old status
	fcntl(fd1,F_SETFL ,save1);
	fcntl(fd2,F_SETFL, save2);
	close(epollfd);
}

	int main(int argc, char * argv[]){
		int fd1 = open(TTY1,O_RDWR);
		if(fd1 < 0){
			perror("open()");
			exit(1);
		}
		int fd2 = open(TTY2,O_RDWR);
		if(fd2 < 0){
			perror("open()");
			exit(1);
		}
		relayer(fd1,fd2);
		close(fd1);
		close(fd2);
		exit(0);
	}



