#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	int i;
	for ( i=0; i<5; i++)
	{
		pid = fork();

		if ( pid == -1 )
		{
			printf("error\n");
			exit(1);
		}
		else if ( pid == 0 )
		{
			break;
		}
	
	}

	if ( i < 5 )
	{
		sleep(i);
		printf("son  %d process ... \n", i);
	}
	else
	{
		sleep(i);
		printf("father %d process \n", i);
	}
	
	return 0;


}


