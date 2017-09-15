#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int a;
	int b;
}exit_t;

void *fuc(void *arg)
{
	exit_t *ret;
	ret = malloc(sizeof(exit_t));
	
	ret->a = 100;
	ret->b = 300;

	pthread_exit((void *)ret);
}

int main()
{
	pthread_t tid;
	exit_t *retval;
	
	pthread_create(&tid, NULL, fuc, NULL);
	pthread_join(tid, (void **)&retval);
	
	printf(" a = %d, b = %d \n", retval->a, retval->b);

	return 0;
}
