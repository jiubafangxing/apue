#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h> 
int main(int argc, char * argv[]){
	if(argc < 2){
		printf("usage error ....");
		exit(1);
	
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("open()");
		exit(1);
	}
	struct stat    fstatp;	
	int statsize = fstat(fd, &fstatp);
	char * readfile;
	readfile = mmap(NULL, fstatp.st_size,PROT_READ,MAP_SHARED, fd, 0);	
	printf("read %s", readfile);
	munmap(readfile, fstatp.st_size);
	close(fd);
	return 0;
}
