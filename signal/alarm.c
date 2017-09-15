#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

void myfunc()
{
	printf("hello world\n");
}

int main()
{
	struct itimerval it, oldit;
	signal(SIGALRM, myfunc); // 注册SIGALRM信号的捕捉处理函数
	
	it.it_value.tv_sec = 5;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec = 3;
	it.it_interval.tv_usec = 0;

	if (setitimer(ITIMER_REAL, &it, &oldit) == -1 )
	{
		perror("setitimer error");
		exit(0);
	}
	while(1);

	return 0;
}
