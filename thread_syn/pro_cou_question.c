#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 定义存放物品容器
struct msg{
	struct msg *next; // 链表
	int num;  // 数据
};

struct msg *head;
struct msg *mp;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // 条件变量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // 互斥锁


void *con(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while (head == NULL)
		{
			pthread_cond_wait(&cond, &mutex);  // 循环查看当前容器内有没有物品，
			/*
			 * 1. 如果没有物品，那么消费者线程全部则色
			 * 2. 解锁mutex
			 * 3. 等待被唤醒，一旦被唤醒。循环消费*/
		}
		mp = head;
		head = mp->next;

		pthread_mutex_unlock(&mutex);
		printf("consume --- %d\n", mp->num);
		free(mp);
		sleep(rand() % 5);

	}

}

// 生产者
void *pro(void *arg)
{
	while(1)
	{
		mp = malloc(sizeof(struct msg));  // 模拟生产物品
		mp->num = rand() % 1000 + 1;
		printf("product --- %d\n", mp->num);

		//  加锁->将物品放入容器->解锁  
		pthread_mutex_lock(&mutex);
		
		mp->next = head;
		head = mp;

		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&cond);  // 唤醒消费者线程

		sleep(rand() % 5);
	}
}

int main()
{
	pthread_t ptid, ctid;  

	pthread_create(&ptid, NULL, pro, NULL);
	pthread_create(&ctid, NULL, con, NULL);

	pthread_join(ptid, NULL);
	pthread_join(ctid, NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}	

