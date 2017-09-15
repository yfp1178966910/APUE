/*
 * 死锁
 *
 * 1. 子进程先锁B，然后在锁A， 
 * 2. 父进程先锁A，然后在锁B
 *
 * */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


pthread_mutex_t mutexA;
pthread_mutex_t mutexB;

void *fuc(void *arg)
{
	srand(time(NULL));
	while(1)
	{
		pthread_mutex_lock(&mutexB);
		printf("fun B\n");
		sleep(rand() % 3);
		//pthread_mutex_unlock(&mutexB);
		//pthread_mutex_lock(&mutexA);
		printf("fun A\n");
		sleep(rand() % 3);
		pthread_mutex_unlock(&mutexA);
	}
	return NULL;
}




int main()
{
	pthread_t tid;
	srand(time(NULL));
	pthread_mutex_init(&mutexA, NULL);  // mutex = 1
	pthread_mutex_init(&mutexB, NULL);  // mutex = 1

	pthread_create(&tid, NULL, fuc, NULL);
	
	while (1)
	{
		pthread_mutex_lock(&mutexA);
		printf("main A\n");
		sleep(rand() % 3);
		//pthread_mutex_unlock(&mutexA);
		//pthread_mutex_lock(&mutexB);
		printf("main B\n");
		sleep(rand() % 3);
		pthread_mutex_unlock(&mutexB);
		
	}
	
	pthread_mutex_destroy(&mutexA);
	pthread_mutex_destroy(&mutexB);

	pthread_exit((void *)0);
}
