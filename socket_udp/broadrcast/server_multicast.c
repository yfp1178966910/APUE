#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <netinet/in.h>

#define SERV_PORT 7777
#define CLIENT_PORT 8888
#define BROADCAST_IP "10.1.10.255"
#define MAXLINE 100
#define GROUP "239.0.0.2"

int main(void)
{
    int sockfd;
    struct sockaddr_in serv_addr, clie_addr;
    char buf[BUFSIZ], clie_buf[BUFSIZ];
    int n;
    struct ip_mreqn group;
    
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
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    
    // 绑定 IP, 端口
    int ret_bind = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret_bind == -1)
    {
        perror("bind error\n");
        exit(1);
    }

    /* 构建组 */
    inet_pton(AF_INET, GROUP, &group.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
    group.imr_ifindex = if_nametoindex("enp3s0");

    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_IF, &group, sizeof(group));
    
    /* 客户端 port */
    bzero(&clie_addr, sizeof(clie_addr));
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(CLIENT_PORT); 
    //clie_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, GROUP, &clie_addr.sin_addr.s_addr);

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
