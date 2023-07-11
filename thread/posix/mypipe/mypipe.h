#ifndef __MY_PIPE_H__
#define __MY_PIPE_H__
#include <stdio.h>;
#define PIPESIZE 1024
typedef void  my_pipe_stat ;

my_pipe_stat *init();

int read(my_pipe_stat *,void *buf, size_t readSize);


int write(my_pipe_stat *,const void * buf,  int readSize);

void destory(my_pipe_stat *);

#endif
