#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666

int main(void)
{
    int sfd, len;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ]; 

    /*创建一个客户端的socket 指定IPv4 TCP*/
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /*默认绑定bind, 系统指定*/

    /* 设定服务器的socket的数据 */
    bzero(&serv_addr, sizeof(serv_addr));                       //清零
    serv_addr.sin_family = AF_INET;                             //IPv4协议族
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    //设定服务端IP 字符串类型转换为网络字节序 参3:传出参数
    serv_addr.sin_port = htons(SERV_PORT);                      //指定端口 本地转网络字节序

    /*发起连接请求*/
    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        /*从标准输入获取数据*/
        fgets(buf, sizeof(buf), stdin);
        /*将数据写给服务器*/
        write(sfd, buf, strlen(buf));       //写给服务器
        /*从服务器读回转换后数据*/
        len = read(sfd, buf, sizeof(buf));
        /*写至标准输出*/
        write(STDOUT_FILENO, buf, len);
    }

    /*关闭链接*/
    close(sfd);

    return 0;
}

