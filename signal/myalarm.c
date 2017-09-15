/*
 *1. 实现alarm， 一个进程只有有一个定时器，当第二次调用alarm时返回上一次定时剩下的时间。
 *
 *2。 可以打印1s内的I/0
 *
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>


// 使用setitimer实现alarm
int myalarm(int sec)
{
	struct itimerval it;
	struct itimerval oldit;
	// int ret;
	it.it_value.tv_sec = sec;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;
	
	setitimer(ITIMER_REAL, &it, &oldit);
	if ( setitimer(ITIMER_REAL, &it, &oldit) == -1 )
	{
		printf("error");
		exit(1);
	}
	return oldit.it_value.tv_sec; // 返回上一次定时剩余的时间
}	

int main()
{
	int i;
	myalarm(1);

	// 打印一秒内的i/o
	for( i=0; ; i++)
		printf("%d\n", i);
	return 0;

}
