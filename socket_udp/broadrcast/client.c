#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>

#define CLIENT_PORT 8888
#define MAXLINE 100

int main(void)
{
    int sfd, n;
    struct sockaddr_in serv_addr, local_addr;
    struct ip_mreqn group;
    ssize_t len;
    char buf[BUFSIZ]; 

    sfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&local_addr, sizeof(local_addr));  
    local_addr.sin_family = AF_INET;                             
    inet_pton(AF_INET, "0.0.0.0", &local_addr.sin_addr.s_addr);    
    local_addr.sin_port = htons(CLIENT_PORT);    

    int ret = bind(sfd, (struct sockaddr *)&local_addr, sizeof(local_addr)); 
    
    inet_pton(AF_INET, "239.0.0.2", &group.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &group.imr_address);
    group.imr_ifindex = if_nametoindex("enp3s0");
    setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &group, sizeof(group));

    while (1) {
        len = recvfrom(sfd, buf, sizeof(buf), 0, NULL, 0);
        write(STDOUT_FILENO, buf, len);
    }

    /*关闭链接*/
    close(sfd);

    return 0;
}

