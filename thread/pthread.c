/*
 * 创建线程
 *
 * */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

// 线程函数
void *thread_fuc(void *arg)
{
	printf("In thread id = %u, pid = %u\n", pthread_self(), getpid());
	return NULL;
}

int main()
{
	pthread_t tid;
	int ret;
	printf("main : thread id = %u, pid = %u\n", pthread_self(), getpid());

	ret = pthread_create(&tid, NULL, thread_fuc, NULL);
	if ( ret != 0 )
	{
		fprintf(stderr, "pthread error: %s\n", strerror(ret));
		exit(1);
	}
	// sleep(1);
	// printf("main : thread id = %u, pid = %u\n", pthread_self(), getpid());
	// return 0;
	pthread_exit((void *)1);
}
