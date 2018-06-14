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
#include "wrap.h"

# define MAXLINE 80
# define SERV_PORT 8888
# define INET_ADDRSTRLEN 16
# define OPEN_MAX 1024
int main(){
    int                 i, maxi, maxfd, listenfd, connfd, sockfd;
    int                 nready;
    ssize_t             n;
    char                buf[BUFSIZ];
    char                str[INET_ADDRSTRLEN];
    socklen_t           cliaddr_len;
    struct sockaddr_in  cliaddr, seraddr;
    struct pollfd       client[OPEN_MAX];

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*) &seraddr, sizeof(seraddr));

    Listen(listenfd, 100);
    
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    
    for(i = 1; i < OPEN_MAX; i++){
        client[i].fd = -1;
    }

    maxi = 0;

    while(1){
        nready = poll(client, maxi+1, -1);
        if(nready < 0){
            printf("poll] error\n");
            exit(-1);
        }

        if (client[0].revents & POLLRDNORM){
            cliaddr_len = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
            printf("received from %s at %d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)),
                    ntohs(cliaddr.sin_port));
            for(i=1; i<OPEN_MAX; i++){
                if (client[i].fd < 0){
                    client[i].fd = connfd;
                    break;
                }
            }
            if (i == OPEN_MAX){
                printf("too many clients\n");
                exit(1);
            }
            client[i].events = POLLRDNORM;
            if (i > maxi)
                maxi = i;
            if (--nready == 0)
                continue;
        }

        for(i = 1; i <= maxi; i++){
            if ((sockfd = client[i].fd) < 0)
                continue;
            if (client[i].events & (POLLRDNORM | POLLERR)){
                if ( (n = Read(sockfd, buf, MAXLINE)) < 0){
                    if (errno == ECONNRESET){
                        printf("RST\n");
                        Close(sockfd);
                        client[i].fd = -1;

                    }
                    else
                        perr_exit("read error");
                }
                else{
                    int j;
                    for(j = 0; j<n; j++)
                        buf[j] = toupper(buf[j]);
                    Write(sockfd, buf, n);
                }
                if (--nready == 0)
                    break;
            }
        }
    }
    Close(listenfd);
    return 0;
}
