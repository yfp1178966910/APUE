/* 
#########################################################################
# File Name: server.c
# Author: yanfeipeng
# address: seclab
# Created Time: Fri 13 Oct 2017 06:30:31 AM EST
#########################################################################
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "protocl.h"


int main(){
    
    //登录列表
    struct list *ls = create_list();

    // 创建serverfifo, 只读打开
    int server = mkfifo(SER_FIFO, 0644);
    if ( server < 0 )
        sys_err("mkfifo");

    int server_fd;
    server_fd = open(SER_FIFO, O_RDONLY);
    if ( server_fd == -1 )
        sys_err("open");

    Proc proc; // 消息封装
    char buf[MAXLINE]={0}; // 缓冲区
    int client_fd;
    
    int test;

    while( 1 ){

        if ( read(server_fd, &proc, sizeof(Proc)) ){
            
            // 消息id 1代表注册。
            if ( proc.id == 1 ){
                
                insert_list(ls, 0, proc.src);
                sprintf(buf, "%s is login\n", proc.src);
                write(STDOUT_FILENO, buf, strlen(buf));

                if ( mkfifo(proc.src, 0644) < 0 ){ // 创建注册用户对应的fifo
                    sys_err("mkfifo client");
                }
                if ( (client_fd = open(proc.src, O_WRONLY)) < 0 ){
                    sys_err("open client");
                }

                memset(&proc, 0, sizeof(Proc));
                proc.id = 1;
                strcpy(proc.data, CONNECT);  // 向用户发送连接成功信号

                write(client_fd, &proc, sizeof(Proc));
                close(client_fd);
            }
            
            // 消息id 2代表转发
            if ( proc.id == 2 ){
                
                if ( (client_fd = open(proc.dst, O_WRONLY)) < 0 ){
// 判断目标用户的fifo是否存在，不存在则说明不在线， 存在则转发消息
                    printf("%d\n", errno);
                    if ( errno == 2 ){  
                        
                        client_fd = open(proc.src, O_WRONLY);

                        memset(&proc, 0, sizeof(Proc));
                        proc.id = 3;
                        strcpy(proc.data, "the user is not online\n");
                        write(client_fd, &proc, sizeof(Proc));
                        
                        close(client_fd);
                    }
                    else{

                        sys_err("open client_fd error\n");
                    }
                }
                else{

                    write(client_fd, &proc, sizeof(Proc));
                    printf("%s->%s:%s", proc.src, proc.dst, proc.data);
                    close(client_fd);
                }
            }
            if (proc.id == 4 ) 
            {
                close(client_fd);
                unlink(proc.src);

                sprintf(buf, "%s is logout\n", proc.src);
                write(STDOUT_FILENO, buf, strlen(buf));

                delete_node(ls, proc.src);

                if (empty_list(ls) == 0 )
                     break;
            }
        }
    }
    printf("server end\n");
    free(ls);
    close(server_fd);
    unlink(SER_FIFO);

    return 0;
}
