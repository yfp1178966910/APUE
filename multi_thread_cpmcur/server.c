/*#########################################################################
# File Name: server.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年05月31日 星期四 16时11分39秒
#########################################################################*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include <sys/wait.h>
#include "wrap.h"

#define SERV_PORT 7777
#define SERV_IP "127.0.0.1"


void *doWork(void *arg){
    int cfd = (int)arg;
    int n, i;
    char buf[BUFSIZ];
    while(1){
        bzero(buf, sizeof(buf));
        n = Read(cfd, buf, sizeof(buf));
        printf("%s\n", buf);
        for(i=0; i<n; i++){
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, n);
    }
    close(cfd);
    return ;
}

int main(void){
    int lfd, cfd[1024];
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    pthread_t tid;
    char buf[BUFSIZ], clie_ip[BUFSIZ];
    int i=0;

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

    Bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    Listen(lfd, 10);
    while(1){
        clie_addr_len = sizeof(clie_addr);
        bzero(&clie_addr, sizeof(clie_addr));
        cfd[i] = Accept(lfd, (struct sockaddr*)&clie_addr, &clie_addr_len);
        printf("client ip : %s, port: %d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_ip, sizeof(clie_ip)),
                ntohs(clie_addr.sin_port));

        pthread_create(&tid, NULL, doWork, (void *)cfd[i]);
        pthread_detach(tid);
        i++;
    }
    return 0;
}
