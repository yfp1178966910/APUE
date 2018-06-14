/*#########################################################################
# File Name: server.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年06月06日 星期三 16时39分20秒
#########################################################################*/
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include "wrap.h"

# define MAXLINE 10
# define SERV_PORT 8888
# define INET_ADDRSTRLEN 16
# define OPEN_MAX 1024

int main(){
    int                 i, j, maxi, epfd, listenfd, connfd, sockfd, res;
    int                 nready;
    ssize_t             n;
    char                buf[MAXLINE];
    char                str[INET_ADDRSTRLEN];
    socklen_t           cliaddr_len;
    struct sockaddr_in  cliaddr, seraddr;
    int                 client[OPEN_MAX];
    struct epoll_event  evt[OPEN_MAX], event;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(SERV_PORT);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    Bind(listenfd, (struct sockaddr*) &seraddr, sizeof(seraddr));

    Listen(listenfd, 100);
    
    connfd = accept(listenfd, (struct sockadd*)&cliaddr, &cliaddr_len);
    printf("received from %s at %d\n",                    
            inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)),
            ntohs(cliaddr.sin_port));

    event.events = EPOLLIN | EPOLLET;
    //event.events = EPOLLIN;
    event.data.fd = connfd;
    epfd = epoll_create(10);
    if (epfd == -1)
        perr_exit("expoll_create");

    int flag = fcntl(connfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(connfd, F_SETFL, flag);

    res = epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
    if (res == -1)
        perr_exit("epoll_ctl");

    bzero(buf, sizeof(buf));
    while(1){
        nready = epoll_wait(epfd, evt, 10, -1);
        if(nready == -1){
            printf("epoll_wait error\n");
            exit(-1);
        }
        if (evt[0].data.fd == connfd){
            Readn(connfd, buf, MAXLINE*4);
            printf("%s", buf);
        }

    }
    Close(listenfd);
    return 0;
}
