/*#########################################################################
# File Name: pthread_10.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月28日 星期三 13时35分03秒
#########################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

var = 100;

void *thread_func(void *arg)
{
    int i = (int)arg;
    sleep(i);
    printf("%d thread id = %u, var = %d, pid = %d\n", i, pthread_self(), var, getpid());
    var++;

    return NULL;
}

int main()
{
    pthread_t tid;
    int ret, i;
    
    void *e_tid;
    printf("main thread id = %u, pid = %d\n", pthread_self(), getpid());

    for (i=0; i<10; i++)
    {
        ret = pthread_create(&tid, NULL, thread_func, (void *)i);
        sleep(i);
        if (ret != 0)
        {
            fprintf(stderr, "pthread errpr: %s\n", strerror(ret));
            exit(1);
        }
        ret = pthread_join(tid, &e_tid);
        printf("thread exit by %d\n", (long)e_tid);
    }
    sleep(10);
    while(1);
    return 0;
}
