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
#include "wrap.h"

# define MAXLINE 80
# define SERV_PORT 8888
# define INET_ADDRSTRLEN 16

int main(){
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset, wset;
    char buf[BUFSIZ];
    char str[INET_ADDRSTRLEN];
    socklen_t cliaddr_len;
    struct sockaddr_in cliaddr, seraddr;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr*) &seraddr, sizeof(seraddr));

    Listen(listenfd, 100);

    maxfd = listenfd;  // 记录监听文件描述符的最大值
    maxi = -1;   // 记录自定数组中的下标位置
    
    for(i = 0; i<FD_SETSIZE; i++){
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_ZERO(&wset);
    FD_SET(listenfd, &allset);
    FD_SET(listenfd, &wset);

    while(1){
        rset = allset; // 更新监听文件描述符集
        nready = select(maxfd+1, &rset, NULL, NULL, NULL); // 只监听读操作
        if(nready < 0){
            printf("select error\n");
            exit(-1);
        }

        if (FD_ISSET(listenfd, &rset)){  // 判断listenfd 是否发生
            cliaddr_len = sizeof(cliaddr);
            connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
            printf("received from %s at %d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr,str,sizeof(str)),
                    ntohs(cliaddr.sin_port));
            for(i=0; i<FD_SETSIZE; i++){
                if (client[i] < 0){
                    client[i] = connfd;  // 发生则建立连接，返回的文件描述符加入管理数组中。
                    break;
                }
            }
            if (i == FD_SETSIZE){
                printf("too many clients\n");
                exit(1);
            }
            FD_SET(connfd, &allset); // 更新总文件描述符集
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)  // 更新最大文件描述符位置
                maxi = i;
            if (--nready == 0)
                continue;
        }

        for(i = 0; i <= maxi; i++){
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)){
                if ( (n = Read(sockfd, buf, MAXLINE)) == 0){  // 对端关闭
                    Close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
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
