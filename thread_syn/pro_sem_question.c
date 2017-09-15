#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
# define NUM 5

sem_t blank_number, pro_number;

int queue[NUM];

void *con(void *arg)
{
	int i = 0;  // 这里的i除了初始化时是0之外，一直不会再次执行到这一步，一直切换的是CPU轮片

	while(1)
	{
		sem_wait(&pro_number);  // 产品数减少
		printf("consume --- %d\n", queue[i]);
		queue[i] = 0;
		sem_post(&blank_number); // 需求增加
		i = (i + 1) % NUM;  // 循环队列
		sleep(rand() % 5);
	}

}

// 生产者
void *pro(void *arg)
{
	int i = 0;

	while(1)
	{
		sem_wait(&blank_number);  // 模拟生产物品
		queue[i] = rand() % 1000 + 1;
		printf("product --- %d\n", queue[i]);

		sem_post(&pro_number);
		
		i = (i + 1) % NUM;

		sleep(rand() % 5);
	}
}

int main()
{
	pthread_t ptid, ctid;  

	sem_init(&blank_number, 0, NUM);
	sem_init(&pro_number, 0, 0);

	pthread_create(&ptid, NULL, pro, NULL);
	pthread_create(&ctid, NULL, con, NULL);

	pthread_join(ptid, NULL);
	pthread_join(ctid, NULL);

	sem_destroy(&blank_number);
	sem_destroy(&pro_number);

	return 0;
}	

