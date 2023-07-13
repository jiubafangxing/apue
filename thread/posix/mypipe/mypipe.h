#ifndef __MY_PIPE_H__
#define __MY_PIPE_H__
#include <stdio.h>;
#define PIPESIZE 1024
#define READ_ROLE  0X00000001UL
#define WRITE_ROLE 0X00000010UL
typedef void  my_pipe_stat ;

my_pipe_stat *init();

int read(my_pipe_stat *,void *buf, size_t readSize);


int write(my_pipe_stat *,const void * buf,  int readSize);

int mypipe_register(my_pipe_stat *, int opmap);

int mypipe_unregister(my_pipe_stat *, int opmap);
void destory(my_pipe_stat *);

#endif
