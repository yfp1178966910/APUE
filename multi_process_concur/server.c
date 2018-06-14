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

void wait_child(int signo){
    
    while(waitpid(0, NULL, WNOHANG) > 0){
        ;
    }

    return ;
}

int main(void){
    int lfd, cfd;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    pid_t pid;
    char buf[BUFSIZ], clie_ip[BUFSIZ];
    int i;

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    Bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    Listen(lfd, 10);
    while(1){
        clie_addr_len = sizeof(clie_addr);
        cfd = Accept(lfd, (struct sockaddr*)&clie_addr, &clie_addr_len);
        printf("client ip : %s, port: %d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_ip, sizeof(clie_ip)),
                ntohs(clie_addr.sin_port));
        pid = fork();
        if (pid < 0){
            perror("fork");
            exit(-1);
        }
        else if (pid == 0){   // 子进程不监听连接，只监听读事件
            close(lfd);  
            break;
        }
        else{
            close(cfd);
            signal(SIGCHLD, wait_child);
        }
    }
    if (pid == 0){
        while(1){
            bzero(buf, sizeof(buf));
            int n = Read(cfd, buf, sizeof(buf));
            if (n == 0){
                close(cfd);
                return 0;
            }
            else if (n == -1){
                perror("read error");
                exit(-1);
            }
            else{
                printf("%s\n", buf);
                for(i=0; i<n; i++){
                    buf[i] = toupper(buf[i]);
                }
                write(cfd, buf, n);
            }

        }
    }
    return 0;
}
