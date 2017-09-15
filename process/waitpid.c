#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	pid_t pid, wpid;
	int i=0;

	for ( i=0; i<5; i++)
	{
		pid = fork();

		if ( pid == 0 )
			break;
		
		if ( pid == -1 )
		{
			printf("fork error\n");
			exit(1);
		}
	}

	if ( i == 5 )
	{
		sleep(5);
		printf("parent .. pid = %u, ppid = %u\n", getpid(), getppid());

		// 轮巡回收子进程
		while( wpid = waitpid(-1, NULL, WNOHANG) > 0 )
			printf("gc  .. wpid = %d\n", wpid);
		
		printf(" finish\n");

		while(1);
	}
	else
	{
		sleep(i);
		printf("child .. pid = %u, ppid = %u\n", getpid(), getppid());
	}

	return 0;
}
