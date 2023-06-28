#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#define BUFSIZE 10
#define BUCK 10
static int LOOP = 0;
static int token = 0;

static void alrm(int sig){
	alarm(1);
	token++;
	if(token > BUCK ){
		token = BUCK;
	}
}
int main(int argc, char *argv[]){
        if(argc < 2){
                printf("call %s, param size is not enough", argv[0]);
        }
        int frd = 0;
        int fwd = 1;
	signal(SIGALRM,alrm);
        alarm(1);
	//int open(const char *pathname, int flags);
        //int open(const char *pathname, int flags, mode_t mode);
        // O_RDONLY, O_WRONLY, or O_RDWR
        frd = open(argv[1],O_RDONLY);
        if(frd < 0){
                perror("open error ");
                exit(1);
        }
//O_CLOEXEC, O_CREAT, O_DIRECTORY, O_EXCL, O_NOCTTY,
//       O_NOFOLLOW, O_TMPFILE, and O_TRUNC.
//  fwd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC, 0666);
// if(fwd < 0){
//                 close(frd);
//                 perror("open error ");
//                exit(1);
//
//}
        char BUF[BUFSIZE];

        while(1){
		while(token < 1){
			pause();
		}
		token--;
		LOOP = 0;
		int ret = 0;
		while((ret = read(frd, BUF, BUFSIZE)) < 0){

                	if(errno == EINTR){
				continue;
			}
			perror("read()");
			break;
		}
                int readed =  0;
                while(1){
                    int writeSize =     write(fwd, BUF+readed ,ret-readed );
                    if(writeSize < 0){
                        exit(1);
                    }
                    if(writeSize == ret)                   {
                        break;
                    } else{
                      readed += writeSize ;
                    }
                }
        }

        close(frd);
}
