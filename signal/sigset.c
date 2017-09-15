/*
 * 打印未决信号集：
 *
 *1. 自定义信号集影响屏蔽字(0, 1)。
 *		信号实质上是也是数值，进行按位运算翻转屏蔽字上的值。
 *2. 读取信号状态
 *
 *3. 当信号产生时，信号被读取，此时刚好打印未决信号集
 * 
 * */



#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>

void printped(sigset_t *ped)
{
	int i;

	// 按位寻找信号屏蔽字的位置
	for( i=0; i<31; i++)
	{
		if(sigismember(ped,i) == 1)
		{
			putchar('1');
		}
		else
			putchar('0');
	}
	printf("... end ... \n");
}

int main()
{
	sigset_t myset, oldset, ped;
	sigemptyset(&myset);
	sigaddset(&myset, SIGQUIT);
	sigaddset(&myset, 5);
	sigaddset(&myset, 7);
	sigaddset(&myset, SIGKILL);
	
	// 使用自定义信号集影响屏蔽信号集。
	if (sigprocmask(SIG_BLOCK, &myset, &oldset) == -1)
	{
		printf("error\b");
		exit(1);
	}
	while(1){	
		sigpending(&ped);  // 读阻塞状态
		printped(&ped);
	}
	return 0;
}
