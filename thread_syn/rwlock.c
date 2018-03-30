/*
 *
 * 读写锁
 * 写独占，读共享。
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int count = 100;
pthread_rwlock_t rwlock;

void *fuc_read(void *arg)
{
	int i = (int)arg;

	while(1)
	{
		pthread_rwlock_rdlock(&rwlock);
		printf("---read---- %d; %lu; count = %d\n", i, pthread_self(), count);
		pthread_rwlock_unlock(&rwlock);
		usleep(500);
	}
	pthread_exit((void *)i);

}

void *fuc_write(void *arg)
{
	int i = (int)arg;

	while(1)
	{
		pthread_rwlock_wrlock(&rwlock);
		printf("---write---- %d; %lu; conuter = %d\n", i, pthread_self(), ++count);
		pthread_rwlock_unlock(&rwlock);
		usleep(800);
	}
	pthread_exit((void *)i);

}

int main()
{
	int i;
	pthread_t tid[8];

	pthread_rwlock_init(&rwlock, NULL);

	for( i=0; i<3; i++)
		pthread_create(&tid[i], NULL, fuc_write, (void *)i);

	for( i=3; i<8; i++)
		pthread_create(&tid[i], NULL, fuc_read, (void *)i);

	for( i=0; i<8; i++)
		pthread_join(tid[i], NULL);

	pthread_rwlock_destroy(&rwlock);
	return 0;
}
