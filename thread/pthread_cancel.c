#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

void *fuc1(void *arg)
{
	printf("thread 1 returning\n");
	return (void *)1;
}

void *fuc2(void *arg)
{
	printf("thread 2 pth_exit\n");
	pthread_exit((void *)2);
}

void *fuc3(void *arg)
{
	while(1)
	{
		pthread_testcancel(); 	
	}
	return (void *)3;
}

int main()
{
	pthread_t tid;
	int i, ret;

	pthread_create(&tid, NULL, fuc1, NULL);
	pthread_join(tid, (void **)&ret);
	printf("%d\n", (int)ret);

	pthread_create(&tid, NULL, fuc2, NULL);
	pthread_join(tid, (void **)&ret);
	//pthread_cancel(tid); // 已经进行了回收，再杀死，则会产生段错误
	printf("%d\n", (int)ret);

	pthread_create(&tid, NULL, fuc3, NULL);
	pthread_cancel(tid); // 杀死线程，需要取消点。一般是系统调用	
	pthread_join(tid, (void **)&ret); // 杀死之后在回收，则回收不到。
	printf("%d\n", (int)ret);

	pthread_exit(NULL);	
}
