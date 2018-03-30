/*#########################################################################
# File Name: clent1.c
# Author: yanfeipeng
# address: seclab
# Created Time: Fri 13 Oct 2017 07:37:44 AM EST
#########################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "list.h"
#include "protocl.h"

// 消息切分
void Cut_message(char str[], char *src, char data[]){
    
    char p[MAXLINE];
    strcpy(p, str);
    char *src1 = strtok(str, ",");
    strcpy(src, src1);
    strcpy(data, p+strlen(src)+1);
}

int main(){

    char name[MAXLINE];

    printf("please input your name: ");
    scanf("%s", name);

    // 设置非阻塞标准输入 用于发消息
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) < 0 ){
        sys_err("fcntl");
    }

    int server_fd = open(SER_FIFO, O_WRONLY);
    if ( server_fd == -1 )
        sys_err("open");

    Proc proc_client; // 消息封装

    // 注册
    proc_client.id = 1;
    strcpy(proc_client.src, name);

    write(server_fd, &proc_client, sizeof(Proc));
    
    sleep(2); // 等待server注册完成，即建立相应的fifo

    int client_fd; // 只读非阻塞打开对应的fifo 以读取数据

    if ( ( client_fd = open(name, O_RDONLY | O_NONBLOCK)) < 0 ){
        sys_err("open");
    }
    char buf[MAXLINE] = {0};
    int size_in;

    char p[MAXLINE], q[MAXLINE]; // 存储消息切分后读字符串，
    while (1)
    {
        memset(&proc_client, 0, sizeof(Proc));

        // 读取 客户端对应的fifo区
        if ( ( size_in = read(client_fd, &proc_client, MAXLINE)) > 0){
            
            // 这里1,3号包 均属于server发回client的包,要么不在线，要么注册成功信息
           if ( proc_client.id == 1 || proc_client.id == 3 ){
               write(STDOUT_FILENO, proc_client.data, strlen(proc_client.data));
           }
            
           if ( proc_client.id == 2 ){

               memset(buf, 0, MAXLINE);
               sprintf(buf, "%s:%s\n", proc_client.src, proc_client.data);
               printf("---%s：%s", proc_client.src, proc_client.data);
               /*
               write(STDOUT_FILENO, proc_client.src, strlen(proc_client.src));
               printf(":");
               write(STDOUT_FILENO, proc_client.data, strlen(proc_client.data));*/
               memset(buf, 0, MAXLINE);
           }

           if ( proc_client.id == 4 ){

               write(STDOUT_FILENO, proc_client.data, strlen(proc_client.data));
               break;
           }
        }
        // else
          //  printf("%s.fifo is empty\n", name);


        // 读入消息
        if ( size_in = read(STDIN_FILENO, buf, MAXLINE) > 0 ){

            if ( strcmp(buf, "exit") == 0 ){
                proc_client.id = 4;
                strcpy(proc_client.src, name);
                write(server_fd, &proc_client, sizeof(Proc));  
            }
            else {

               //memset(buf, 0, MAXLINE);
               proc_client.id = 2;

               strcpy(proc_client.src, name);
               //printf("%s\n", buf);
               Cut_message(buf, p, q);

               strcpy(proc_client.dst, p);
               strcpy(proc_client.data, q);
               write(server_fd, &proc_client, sizeof(Proc));
               memset(buf, 0, MAXLINE);
            }
        }

        sleep(1);
        
    }
    close(server_fd);
    close(client_fd);
    
    printf("client end\n");
    return 0; 
}
