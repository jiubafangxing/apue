#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include "proto.h"
int main(int argc, char * argv[]){
    int socketFd =  socket(AF_INET,SOCK_DGRAM,0);
    if(socketFd < 0){
    	perror("socket()");
	exit(1);
    }
    int flag = 1;
    int opResult = setsockopt(socketFd,SOL_SOCKET,SO_BROADCAST,&flag,sizeof(flag));
    if(opResult < 0){
    	close(socketFd);
    	perror("setsockopt...");
	exit(1);
    }
    struct sockaddr_in si ,remoteAddr;
    si.sin_family = AF_INET;
    struct in_addr  addr ;
    char * ip = "0.0.0.0";
    inet_pton(AF_INET, ip,&addr);
    si.sin_addr = addr;
    si.sin_port = htons(atoi("8090"));
    int result = bind(socketFd,(void *)&si,sizeof(si));
    if(result < 0){
	perror("bind error()");
	exit(1);
    }
    struct msg_st * msgst;

    int bufsize = sizeof(struct msg_st)+MAXSIZE -1;
    msgst = malloc(bufsize);
    socklen_t slt ;
    slt = sizeof(remoteAddr);
    char * ipstr;
    while(1){
    	recvfrom(socketFd, msgst, bufsize,MSG_TRUNC,  (void *)&remoteAddr,&slt);
	printf("ths name is %s\n", msgst->name);
	printf("ths chinese is %d\n", msgst->chinese);
	printf("the math is %d\n", msgst->math);
    }
    close(socketFd);
    return 0;
}
