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
#include "wrap.h"

# define MAXLINE 80
# define SERV_PORT 8888
# define INET_ADDRSTRLEN 16
# define OPEN_MAX 1024

int main(){
    int                 i, j, maxi, epfd, listenfd, connfd, sockfd, res;
    int                 nready;
    ssize_t             n;
    char                buf[BUFSIZ];
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

    Bind(listenfd, (struct sockaddr*) &seraddr, sizeof(seraddr));

    Listen(listenfd, 100);

    event.events = EPOLLIN;
    event.data.fd = listenfd;
    epfd = epoll_create(10);
    if (epfd == -1)
        perr_exit("expoll_create");

    res = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &event);
    if (res == -1)
        perr_exit("epoll_ctl");


    while(1){
        nready = epoll_wait(epfd, evt, OPEN_MAX, -1);
        if(nready == -1){
            printf("epoll_wait error\n");
            exit(-1);
        }
        for(i=0; i<nready; i++){
            if (!(evt[i].events & POLLIN))
                continue;
            if(evt[i].data.fd == listenfd){
                cliaddr_len = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
                printf("received from %s at %d\n",
                        inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)),
                        ntohs(cliaddr.sin_port));

                event.events = EPOLLIN;
                event.data.fd = connfd;
                res = epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &event);
                if (res == -1)
                    perr_exit("epoll_ctl");
            }
            else{
                sockfd = evt[i].data.fd;
                n = Read(sockfd, buf, MAXLINE);
                if( n == 0){
                    res = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
                    if (res == -1)
                        perr_exit("expoll_ctl");
                    Close(sockfd);
                }
                else{
                    for(j=0; j<n; j++)
                        buf[j] = toupper(buf[j]);
                    Write(sockfd, buf, n);
                }
            }
        }
        
    }
    Close(listenfd);
    return 0;
}
