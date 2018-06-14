#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define SERV_PORT 7777
#define CLIENT_PORT 8888
#define BROADCAST_IP "10.1.10.255"
#define MAXLINE 100


/* 程序正确， 本机不行 */
int main(void)
{
    int sockfd;
    struct sockaddr_in serv_addr, clie_addr;
    char buf[BUFSIZ], clie_buf[BUFSIZ];
    int n;
    
    // 建立一个服务端的socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        perror("socket error\n");
        exit(1);
    }
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY 指定一个任意地址，且是数值类型
    
    // 绑定 IP, 端口
    int ret_bind = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret_bind == -1)
    {
        perror("bind error\n");
        exit(1);
    }
    int flag = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));

    bzero(&clie_addr, sizeof(clie_addr));
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(CLIENT_PORT); 
    //clie_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, BROADCAST_IP, &clie_addr.sin_addr.s_addr);

    int i=0;
    while(1){
        printf("test\n");
        sprintf(buf, "this is %dth test\n", i++);
        n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&clie_addr, sizeof(clie_addr));
        sleep(1);
    }

    close(sockfd);
    return 0;
}
