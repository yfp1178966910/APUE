/*#########################################################################
# File Name: protocl.h
# Author: yanfeipeng
# address: seclab
# Created Time: Fri 13 Oct 2017 11:06:19 PM EST
#########################################################################*/

#ifndef __PROTOCL__
#define __PROTOCL__

typedef struct _Protocol{
    int id; // 消息类型
    char src[4];
    char dst[4];
    char data[1024];
}Proc;

void sys_err(char *s);

#define MAXLINE 1024
#define SER_FIFO "SER_FIFO"
#define CONNECT "\
------------------------------------------------------\n\
--  you have connevt with the server successfully   --\n\
--         send message format <dst,message>        --\n\
------------------------------------------------------\n"

#endif

void sys_err(char *s){
    exit(-1);
    printf("%s error \n", s);
}
