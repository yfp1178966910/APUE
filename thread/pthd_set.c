/*
 * 从堆内存中获取空间
 *
 * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define SIZE 0x100000000

void *fuc(void *arg)
{
	while(1)
		sleep(1);
}

int main()
{
	int i = 1, detachstate, ret;
	pthread_t tid;
	pthread_attr_t attr;
	size_t stacksize;
	void *stackaddr;

	pthread_attr_init(&attr);
	pthread_attr_getstack(&attr, &stackaddr, &stacksize); // 获取栈空间大小及地址
	pthread_attr_getdetachstate(&attr, &detachstate); // 获取当前线程状态
	
	if ( detachstate == PTHREAD_CREATE_DETACHED )
		printf(" thread detached \n");
	else if ( detachstate == PTHREAD_CREATE_JOINABLE )
		printf(" thread joinable \n");
	else
		printf(" thread unknow \n");

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	
	
	while (1)
	{
		/*  从堆内存中申请空间  */
		stackaddr = malloc(SIZE); // 指向申请的地址
		if ( stackaddr == NULL )
		{
			perror("malloc error\n");
			exit(1);
		}
		stacksize = SIZE;

		// 设置线程空间大小
		pthread_attr_setstack(&attr, stackaddr, stacksize); 

		ret = pthread_create(&tid, &attr, fuc, NULL);
		if ( ret != 0 )
		{
			printf("%s\n", strerror(ret));
			exit(1);
		}
		printf("%d\n", i++);
	} 
	
	pthread_attr_destroy(&attr);
	return 0;	
}
