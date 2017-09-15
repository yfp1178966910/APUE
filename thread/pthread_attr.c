#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void *thread_fuc(void *arg)
{
	pthread_exit((void *)77);
}

int main()
{
	pthread_t tid;
	int ret;
	pthread_attr_t attr;
	pthread_attr_init(&attr); // 线程属性初始化
	
	// 设置线程分离
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	ret = pthread_create(&tid, &attr, thread_fuc, NULL);
	if ( ret != 0 )
	{
		fprintf(stderr, "pthread error: %s\n", strerror(ret));
		exit(1);
	}

	ret = pthread_join(tid, NULL); // 线程属性是分离，则进行回收返回错误号
	if ( ret != 0 )
	{
		fprintf(stderr, "pthread join error: %s\n", strerror(ret));
		exit(1);
	}
	printf("join ret = %d\n", ret);
	pthread_exit((void *)1);
}
