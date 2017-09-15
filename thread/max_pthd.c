#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

void *fuc(void *arg)
{
	while(1)
		sleep(1);

}

int main()
{
	int i, count = 1;
	pthread_t tid;
	
	for(;;)
	{
		i = pthread_create(&tid, NULL, fuc, NULL);
		if ( i != 0 )
		{
			fprintf(stderr, "%d\n", i);
			//exit(1);
			break;	
		}
		printf("%d th thread\n", count++);
	}
	
	pthread_exit((void *)1);
}

