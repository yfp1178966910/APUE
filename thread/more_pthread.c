/*
 * 循环创建子线程
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
	printf("%d thread id = %u, var = %d\n", i+1, pthread_self(), var);
	var++;
	return NULL;
}

int main()
{
	pthread_t tid;
	int ret, i;
	printf("main : thread id = %u, pid = %u\n", pthread_self(), getpid());
	for(i=0; i<5; i++ )
	{
		ret = pthread_create(&tid, NULL, thread_fuc, (void *)i);
		// ret = pthread_create(&tid, NULL, thread_fuc, (void *)&i);
		// 传地址的话，当thread_fuc读取这个地址值的时候i的值可能会发生改变
		// （void *） 类型变大，变成8字节（64位），小转大。
		
		if ( ret != 0 )
		{
			fprintf(stderr, "pthread error: %s\n", strerror(ret));
			exit(1);
		}	
	}
	sleep(i);

	return 0;
}
