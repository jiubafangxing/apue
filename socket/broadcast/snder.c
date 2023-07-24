#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <string.h>
#include "proto.h"
int main(int argc, char * argv[]){
    if(argc <2 ){
    	perror("usage ...");
	exit(1);
    }
    int socketFd =  socket(AF_INET,SOCK_DGRAM,0);
    int flag = 1;

    int opResult = setsockopt(socketFd,SOL_SOCKET,SO_BROADCAST,&flag,sizeof(flag));
    if(opResult < 0){
    	close(socketFd);
    	perror("setsockopt...");
	exit(1);
    }
    if(socketFd < 0){
    	perror("socket()");
	exit(1);
    }
    struct sockaddr_in si ;
    si.sin_family = AF_INET;
    struct in_addr  addr;
    char * ip = "255.255.255.255";
    inet_pton(AF_INET, ip,&addr);
    si.sin_port = htons(atoi("8090"));
    si.sin_addr = addr;
    struct msg_st * mst;
    
    int bufsize = sizeof(struct msg_st)  + strlen(ip);
    mst = malloc(bufsize);
    strcpy(mst->name, argv[1]);
    mst->chinese = 12;
    mst->math = 30;
    
    int sendResult = sendto(socketFd,(void *) mst,bufsize,0,&si, sizeof(si));
    if(sendResult < 0){
    	perror("send fail");
	exit(1);
    }
    close(socketFd);
    return 0;
}
