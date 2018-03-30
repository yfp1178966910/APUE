/*#########################################################################
# File Name: wait.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月18日 星期日 21时05分12秒
#########################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    pid_t pid, wpid;
    

    pid = fork();
    int status;

    if(pid == 0)
    {   
        sleep(10);
        printf("---child--- \n");
        printf("---child die---\n");
    }
    else if(pid > 0)
    {
        wpid = wait(&status); // 阻塞调用者。等待一个子进程的结束。
        if(wpid == -1)
        {
            printf("wait error\n");
            exit(0);
        }

        if(WIFEXITED(status))
        {
            printf("child exit with %d\n", WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status))
            printf("child killed by %d\n", WTERMSIG(status));
        
        // sleep(10);
        printf("parent --- \n");
    }
    return 0;
}
