/*#########################################################################
# File Name: pipe.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月27日 星期二 15时13分46秒
#########################################################################*/

/*
 * 管道
 * 1. 写段流入，读端流出。
 * 2. 大小是4K，其本质是一个伪文件，缓冲区，借助队列实现。
 * 
 * 局限;
 * 1. 单向
 * 2. 一旦读走，数据消失
 * 3. 半双工
 * 4. 只能在又公共祖先的进程间使用管道。即有血缘关系的进程
 *
 *
 * */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd[2];
    int ret = pipe(fd);
    int red;
    if (ret == -1)
    {
        perror("error\n");
        exit(1);
    }

    pid_t pid = fork();
    
    if (pid == 0)
    {
        // sleep(10);
        close(fd[1]);
        char buf[1024];
        red = read(fd[0], buf, sizeof(buf));
        if (red == 0)
        {
            perror("read error\n");
            exit(1);
        }
        printf("%s\n\n", buf);
        write(STDOUT_FILENO, buf, ret);
    }
    else
    {
        sleep(2);
        close(fd[0]);
        write(fd[1], "hello world\n", strlen("hello world\n"));
    }

    return 0;
}
