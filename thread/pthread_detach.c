#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

void *fuc(void *arg)
{
	printf("test\n");
	pthread_exit((void *)1);
}


int main()
{
	pthread_t tid;
	int i, ret;

	pthread_create(&tid, NULL, fuc, NULL);
	pthread_detach(tid); // 分离状态，这时如果在进行回收的话会报错22
				// 因为已经状态已经被回收了，防止浪费系统资源
	
	ret = pthread_join(tid, (void **)i);
	if ( ret != 0 )
		fprintf(stderr, "%d\n", ret); // 失败返回错误编号
	pthread_exit(NULL);	
}
