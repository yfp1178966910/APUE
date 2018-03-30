/*#########################################################################
# File Name: pro_10.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月18日 星期日 21时35分56秒
#########################################################################*/


/*
 * fork 并不是完全复制父进程的代码部分。而是复制进程控制块
 *
 * 共享;
 * 1. data段 正文, 堆栈，环境变量，信号处理方式，工作目录
 * 2. 独享全局变量， 无法进行通信
 * 3. 写时复制，读时共享。
 *
 * 不共享:
 * 1. 进程运行时间
 * 2. 进程ID
 * 3. 闹钟
 * 4. 未决信号集。
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int i;
    pid_t pid, wpid;
    int status;
    printf("before fork\n");

    for(i=0; i<10; i++)
    {
        pid = fork();
        if(pid == 0) // 子进程结束, 父进程不操作
            break;
    }
    if(i<10)
    {
        sleep(i);
        printf("child %d, pid=%d, ppid=%d\n", i, getpid(), getppid());
    }
    else
    {
        sleep(2);
        // wpid = wait(&status);  // 仅仅能阻塞一个子进程。
                            // 第一个子进程结束后，后面的子进程被init进程回收。
        while( wpid = waitpid(-1, NULL, WNOHANG) > 0); // 循环探测子进程状态
                                                        // 需要注意
                                                        // 某一子进程在执行的过程中，CPU轮转到父进程
                                                        // 则探测不到子进程退出状态。 
                                                        // 后面的子进程就无法进行回收。
        printf("parent %d pid=%d\n",i, getpid());

        if (WIFEXITED(status))
            printf("child exit with %d\n", WEXITSTATUS(status));
        if (WIFSIGNALED(status))
            printf("child killed by %d\n", WTERMSIG(status));

        while(1)
            sleep(2);
    }
    /*
    while(1)
    {
        sleep(3);
        printf("this is a test\n");
    }
    */

    return 0;
}
