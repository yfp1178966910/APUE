#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

typedef struct {
	char ch;
	int var;
	char str[64];
}exit_t;

void *thread_fuc(void *arg)
{
	exit_t *retval = (exit_t *)arg;

	retval->ch = 'n';
	retval->var = 100;
	strcpy(retval->str, "my thread");

	pthread_exit((void *)retval);	
}

int main()
{
	pthread_t tid;
	int ret, i;
	exit_t *retval = (exit_t *)malloc(sizeof(exit_t));

	ret = pthread_create(&tid, NULL, thread_fuc, (void *)retval);
	if ( ret != 0 )
	{
		fprintf(stderr, "pthread error: %s\n", strerror(ret));
		exit(1);
	}	

	pthread_join(tid, (void **)&retval);
	printf("ch = %c, var = %d, str = %s\n", retval->ch, retval->var, retval->str);

	free(retval);
	pthread_exit(NULL);
}
