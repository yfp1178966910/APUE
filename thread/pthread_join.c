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
    int i = (int)arg;
	exit_t *ret;
	ret = malloc(sizeof(exit_t));
	
    // 退出状态
	ret->a = 100;
	ret->b = 300;

	pthread_exit((void *)ret);
}

int main()
{
	pthread_t tid, tid2;
	exit_t *retval; // 接收退出状态
	
    int i = 0;
	pthread_create(&tid, NULL, fuc, (void *)i++);
    // pthread_create(&tid2, NULL, fuc, (void *)i)
	pthread_join(tid, (void **)&retval);
	// pthread_join(tid, (void **)&retval);
	
	printf(" a = %d, b = %d \n", retval->a, retval->b);

	return 0;
}
