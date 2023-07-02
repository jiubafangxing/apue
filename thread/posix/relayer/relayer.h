#ifndef __RELAYER_H
#define __RELAYER_H

enum task_state{
	RUNNING = 1,
	CANCEL,
	OVER,
	
};

struct relayer_st_r{
   int fd1;
   int fd2;
   int task_state;
};

/***
 * 初始化中继引擎
 * 如果返回结果大于零则表示初始化成功
 * 如果小于零
 * 	== -EINVAL 参数非法
 * 	-- -ENOSPC 任务数组满
 * 	-- -ENOMEM 失败，内存分配失败
 *
 * **/
int relayer_init(int fd1, int fd2);

/***
 * 返回结果为0 结束成功
 * 如果返回值非零
 * 	-- -EINVAL 参数非法
 * 	-- -EBUSY 任务失败，任务早已经被结束
 *
 *
 * */
int relayer_cancel(int task);

#endif
