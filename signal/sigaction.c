#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
void catch(int signum)
{
	printf("catch %d success\n", signum);
	sleep(10);
	printf("... end ...\n");
}

int main()
{	
	struct sigaction act, oldact;
	act.sa_handler = catch;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGQUIT);
	act.sa_flags = 0; // 默认属性： 函数执行期间自动屏蔽本信号
	if (sigaction(SIGINT, &act, NULL) == -1)
	{
		printf("sigaction error\n");
		exit(1);
	}
	while(1);
	return 0;
}
