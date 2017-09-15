/*
 *
 * 1. 自定义信号集影响屏蔽字，并且加入了SIGALRM信号，同事也定义了一个信号集，删掉了SIGALRM信号。
 * 2. alarm函数后，由于竟态时序问题，那么会一直嵌入睡眠，接收不到SIGALRM信号。
 * 3. 程序运行到sigsuspend函数，此时使用无SIGALRM信号的信号集。
 * 4. sigsuspend调用结束，开始使用有SIGALRM的信号集，则此时进程会受到SIGALRM信号，程序被唤醒。
 *
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
	sleep(seconds);
	sigsuspend(&suspmask);	

	
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
