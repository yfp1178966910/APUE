#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid = fork();
	if ( pid == -1 )
	{
		printf("fork error\n");
		exit(1);
	}
	else if ( pid == 0)
	{
		printf(" -- child -- parent is %d\n", getppid());
		sleep(10);
		printf(" -- child die -- \n");
	}
	else
	{
		while(1)
		{
			printf("parent .. \n");
			sleep(1);
		}
	}
	return 0;
}
