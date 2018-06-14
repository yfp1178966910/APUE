/*#########################################################################
# File Name: daemon.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年06月13日 星期三 19时16分37秒
#########################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

int main(){
    int pid;
    time_t t;
    if (pid = fork())
        exit(-1);
    else if (pid < 0)
        exit(-1);
    
    setsid();

    if (pid = fork())
        exit(-1);
    else if (pid < 0)
        exit(-1);

    chdir("/tmp/");
    umask(0);

    int fd = open("time.log", O_RDWR | O_CREAT | O_TRUNC, 0644);
    while(1){
        sleep(5);
        if (fd == -1){
            printf("open error");
            exit(-1);
        }
        dup2(fd, STDOUT_FILENO);
        execlp("date", "date", NULL);
    }
    printf("end\n");
    return 0;
}
