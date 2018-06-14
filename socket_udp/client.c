#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERV_PORT 7777
#define CLIENT_PORT 8888
#define MAXLINE 100

int main(void)
{
    int sfd, len, n;
    struct sockaddr_in serv_addr, local_addr;
    char buf[BUFSIZ]; 

    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));  
    bzero(&local_addr, sizeof(local_addr));  

    local_addr.sin_family = AF_INET;                             
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr.s_addr);    
    local_addr.sin_port = htons(CLIENT_PORT);    

    int ret = bind(sfd, (struct sockaddr *)&local_addr, sizeof(local_addr)); 
    

    while (1) {
        len = recvfrom(sfd, buf, sizeof(buf), 0, NULL, 0);
        printf("buf\n");
        /*写至标准输出*/
        write(STDOUT_FILENO, buf, len);
    }

    /*关闭链接*/
    close(sfd);

    return 0;
}

