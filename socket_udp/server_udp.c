#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 7777
#define MAXLINE 100

int main(void)
{
    int sockfd;
    struct sockaddr_in serv_addr, clie_addr;
    char buf[BUFSIZ], clie_buf[BUFSIZ];
    int n, i;
    
    // 建立一个服务端的socket
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

    while(1){
        socklen_t clie_addr_len = sizeof(clie_addr);
        n = recvfrom(sockfd, buf, MAXLINE, 0, (struct sockaddr*)&clie_addr, &clie_addr_len);
        if (n == -1){
            perror("recvfrom error\n");
        }

        printf("client ip:%s, port:%ld \n", 
                inet_ntop(AF_INET, &clie_addr.sin_addr, clie_buf, sizeof(clie_buf)), 
                ntohs(clie_addr.sin_port));

        for (i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);
        n = sendto(sockfd, buf, n, 0, (struct sockaddr*)&clie_addr, clie_addr_len);
        if (n == -1)
            perror("sendto error");

    }

    close(sockfd);
    return 0;
}
