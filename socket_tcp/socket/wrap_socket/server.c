#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666

int main(void)
{
    int lfd, cfd;
    struct sockaddr_in serv_addr, clie_addr;
    char buf[BUFSIZ], clie_buf[BUFSIZ];
    int n, i;
    
    // 建立一个服务端的socket
    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1)
    {
        perror("socket error\n");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); 
    
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);    //设定服务端IP 字符串类型转换为网络字节序 参3:传出参数
    // serv_addr.sin_addr.s_addr = inet_pton(SERV_IP);
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY 指定一个任意地址，且是数值类型
    // INADDR_ANY = inet_addr("0.0.0.0")  将一个点分十进制转换成长整型
    
    // 绑定 IP 端口
    int ret_bind = bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (ret_bind == -1)
    {
        perror("bind error\n");
        exit(1);
    }
    // 设置同时连接数量
    int ret_listen = listen(lfd, 128);
    if (ret_listen == -1)
    {
        perror("listen error\n");
        exit(1);
    }

    socklen_t clie_addr_len = sizeof(clie_addr);

    // 阻塞等待客户端连接，成功返回与客户端连接的文件描述符
    cfd = accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
    if (cfd == -1)
    {
        perror("accept error\n");
        exit(1);
    }
    printf("client ip:%s, port:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_buf, sizeof(clie_buf)),
            ntohs(clie_addr.sin_port));

    while (1) {
        n = read(cfd, buf, sizeof(buf));  // 读取客户端传进来的数据
        for (i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);
        write(cfd, buf, n);  // 写给客户端数据
    }

    close(lfd);
    close(cfd);

    return 0;
}
