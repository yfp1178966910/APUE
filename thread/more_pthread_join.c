/*
 * 循环回收子线程
 *
 * */


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int var = 100;

void *thread_fuc(void *arg)
{
	int i = (int)arg;
	sleep(i);

	if ( i == 1 )
	{
		var = 222;
		printf("%d thread , var = %d\n", i, var);
		return (void *)var;
	}
	else if ( i == 3 )
	{
		var = 444;
		printf("%d thread , var = %d\n", i, var);
		pthread_exit((void *)var);
	}
	else
	{
		printf("%d thread , var = %d\n", i, var);
		pthread_exit((void *)var);
	}

	return NULL;
}

int main()
{
	pthread_t tid[5];
	int *ret[5], i;
	
	for(i=0; i<5; i++ )
	{
		pthread_create(&tid[i], NULL, thread_fuc, (void *)i);
		// 用i 来表示线程序号，
	}
	for(i=0; i<5; i++)
	{
		pthread_join(tid[i], (void **)&ret[i]);
		// ret用来存储线程退出值(void *)类型，所以用二级指针接收，并且指向地址值
		// 所以需要用 取地址值，并且ret的定义是指针类型。
		printf("-----%d ret = %d\n", i, (int)ret[i]);
	}

	pthread_exit(NULL);
}
