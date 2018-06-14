/*#########################################################################
# File Name: epoll_pipe.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年06月06日 星期三 22时35分40秒
#########################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>

#define MAXLINE 10

int main(){
    int epfd, i;
    int pfd[2];
    pid_t pid;
    char buf[MAXLINE], ch = 'a';
    
    bzero(buf, sizeof(buf));
    pipe(pfd);
    pid = fork();
    if(pid == 0){
        close(pfd[0]);
        while(1){
            for(i=0; i<MAXLINE/2;i++)
                buf[i] = ch;
            buf[i-1] = '\n';
            ch++;
            for(; i<MAXLINE;i++)
                buf[i] = ch;
            buf[i-1] = '\n';
            ch++;
            write(pfd[1], buf, sizeof(buf));
            sleep(2);
        }
        close(pfd[1]);
    }
    else if (pid > 0){
        close(pfd[1]);
        struct epoll_event event;
        struct epoll_event evt[10];
        int res, len;
        
        epfd = epoll_create(10);
        event.events = EPOLLIN | EPOLLET;
        //event.events = EPOLLIN;
        event.data.fd = pfd[0];
        epoll_ctl(epfd, EPOLL_CTL_ADD, pfd[0], &event);
        while(1){
            res = epoll_wait(epfd, evt, 10, -1);
            if (evt[0].data.fd == pfd[0]){
                len = read(pfd[0], buf, MAXLINE/2);
                printf("%s", buf);
            }
        }
        close(pfd[0]);
        close(epfd);
    }
    else
        perror("fork");
    return 0;
}
