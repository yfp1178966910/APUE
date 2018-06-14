#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 7777
#define MAXLINE 100

int main(void)
{
    int sfd, len, n;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ]; 

    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));  

    serv_addr.sin_family = AF_INET;                             
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr);    
    serv_addr.sin_port = htons(SERV_PORT);    

    socklen_t serv_addr_len = sizeof(serv_addr);
    while (1) {
        /*从标准输入获取数据*/
        fgets(buf, sizeof(buf), stdin);
        /*将数据写给服务器*/
        n = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr*)&serv_addr, serv_addr_len);
        if (n == -1){
            perror("sendto error");
        }
        /*从服务器读回转换后数据*/
        len = recvfrom(sfd, buf, MAXLINE, 0, NULL, 0);
        /*写至标准输出*/
        write(STDOUT_FILENO, buf, len);
    }

    /*关闭链接*/
    close(sfd);

    return 0;
}

