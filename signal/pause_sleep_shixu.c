/*
 * 竟态时序问题：
 * alarm函数后，由于竟态时序问题，CPU被抢，alarm结束后产生的信号被忽略，进程会一直陷入睡眠。
 *
 * 解决： sigsuspend(原子操作)
 *
 * 1. 自定义信号集影响屏蔽字，并且加入了SIGALRM信号，同时也定义了一个信号集，删掉了SIGALRM信号。
 * 2. sigsuspend之前阻塞SIGALRM信号，产生此信号便处于待处理状态。
 * 3. CPU恢复后，此时解除对SIGALRM信号的屏蔽，
 * 4. 此时进程会开始处理SIGALRM信号。
 *
 * 考虑使用sigsuspend 这个原子操作的函数原因？
 * 若从一开始便阻塞SIGALRM信号，等到CPU恢复后解除阻塞，也可以解决SIGALRM这个问题。
 * 但是，此程序是针对SIGALRM信号。信号产生的时刻是固定的，
 * 若信号产生在解除阻塞和进程唤醒之间，则还是会陷入竟态时序问题。
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

	struct sigaction act, oldact;
	sigset_t newmask, oldmask, suspmask;
	
	act.sa_handler = catch;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	//在屏蔽信号集中屏蔽SIGALRM信号，oldmask保存原来的屏蔽信号集
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask); // 将newmask->oldmask

	ret = sigaction(SIGALRM, &act, &oldact); // 注册捕捉函数
	if (ret == -1)
	{
		printf("sigaction error\n");
		exit(1);
	}
	
	alarm(seconds);

	// 确保sigsuspend函数中使用的屏蔽信号集中SIGALRM是未被屏蔽的
	suspmask = oldmask; 
	sigdelset(&suspmask, SIGALRM);
	/*
 * 	sigsuspend调用期间，采用临时阻塞信号suspmask替换原有的阻塞信号集，
 * 	这个信号中屏蔽SIGALRM信号，当sigsuspend被信号唤醒返回时，恢复原有的阻塞信号集， sigsuspend是一个系统函数，是一个原子操作。
 * 	*/
	sleep(seconds); // cpu被抢，此时信号字内有SIGALRM, 并且alarm产生了SIGALRM信号，被屏蔽，处于等待处理状态
	sigsuspend(&suspmask);	// cpu返回之后， 恢复原来的信号字，不再屏蔽SIGALRM，所以开始处理SIGALRM信号。

	
	ret = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);
	sigprocmask(SIG_UNBLOCK, &oldmask, NULL);

	return ret;
}
int main()
{
	printf("... start ...\n");
	while (1)
	{	
		mysleep(3);
		printf("sleeping ...\n");
	}
	return 0;
}
