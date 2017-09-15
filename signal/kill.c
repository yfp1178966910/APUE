#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
	int i,q;
	for(i=0; i<5; i++)
	{
		int pid = fork();
		if ( pid == 0 )
		{
			break;
		}
		if ( i == 2 )
			q = pid;
	}
	if ( i < 5 )
	{
		while(1)
		{
			printf("child %d, getpid = %u\n", i, getpid());
			sleep(1);
		}
	}
	else if ( (kill(q, SIGKILL)) < 0 )
		{
		exit(-1);
		}
	return 0;
}
