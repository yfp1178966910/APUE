#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


pthread_mutex_t mutex;

void *fuc(void *arg)
{
	srand(time(NULL));
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("hello");
		sleep(rand() % 3);
		printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand() % 3);
	}
	return NULL;
}




int main()
{
	pthread_t tid;
	srand(time(NULL));
	pthread_mutex_init(&mutex, NULL);  // mutex = 1

	pthread_create(&tid, NULL, fuc, NULL);
	
	while (1)
	{
		pthread_mutex_lock(&mutex);
		printf("HELLO");
		sleep(rand() % 3);
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex); // 共享资源是stdout，所以共享资源一结束就解锁
									// 如果在后面的sleep之后解锁，那么很大几率是本while
									// 循环持续拿到锁
		sleep(rand() % 3);
		
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_exit((void *)0);
}
