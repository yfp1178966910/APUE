#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 8888
#define MAXLINE 10

int main(void)
{
    int sfd, len;
    struct sockaddr_in serv_addr;
    char buf[MAXLINE], ch = 'a'; 

    /*创建一个客户端的socket 指定IPv4 TCP*/
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /*默认绑定bind, 系统指定*/

    /* 设定服务器的socket的数据 */
    bzero(&serv_addr, sizeof(serv_addr));                       //清零
    serv_addr.sin_family = AF_INET;                             //IPv4协议族
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    //设定服务端IP 字符串类型转换为网络字节序 参3:传出参数
    serv_addr.sin_port = htons(SERV_PORT);                      //指定端口 本地转网络字节序

    /*发起连接请求*/
    int con = connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (con == -1){
        perror("connect error\n");

    }
    while (1) {
        /*从标准输入获取数据*/
        //fgets(buf, sizeof(buf), stdin);
        /*将数据写给服务器*/
        bzero(buf, sizeof(buf));
        int i;
        for(i=0; i<MAXLINE/2; i++)
            buf[i] = ch;
        buf[i-1] = '\n';
        ch++;
        for(; i<MAXLINE;i++)
            buf[i] = ch;
        buf[i-1] = '\n';
        ch++;
        write(sfd, buf, MAXLINE);       //写给服务器
        /*从服务器读回转换后数据*/
        //len = read(sfd, buf, sizeof(buf));
        /*写至标准输出*/
        //write(STDOUT_FILENO, buf, len);
        sleep(2);
    }

    /*关闭链接*/
    close(sfd);

    return 0;
}

