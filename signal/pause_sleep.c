/*
 *1. 实现sleeo
 *2. akarm函数后有可能产生竟态时序问题。
 *
 * */



#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

// 捕捉函数
void catch(int signo)
{
	printf("this is a catch\n");
}

unsigned int mysleep(unsigned int seconds)
{
	int ret;


	// sigaction的初始化
	struct sigaction act, oldact;
	act.sa_handler = catch;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	ret = sigaction(SIGALRM, &act, &oldact); // 注册捕捉函数
	if (ret == -1)
	{
		printf("sigaction error\n");
		exit(1);
	}
	
	alarm(seconds); // 会产生竟态时序问题，因为调用完定时器后CPU背抢走，那么SIGALRM就回提前产生，但是pasue接收不到，就不会被唤醒。

	// sleep(3);  // 3s 和 1s 是不同的。  测试CPU被抢走的情况以重现竟态时序问题。

	ret = pause();  // pasue执行条件是，等到信号唤醒，并且信号的处理动作被捕捉，
	if (ret == -1 && errno == EINTR)
	{
		printf("pause success\n");
	}
	
	ret = alarm(0);  // 清空计时器
	sigaction(SIGALRM, &oldact, NULL);  // 清理空间

	return ret;
}


int main()
{
	printf("... start ...\n");
	while (1)
	{	
		mysleep(5);
		printf("sleeping ...\n");
	}
	return 0;
}
