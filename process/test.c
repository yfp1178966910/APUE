/*#########################################################################
# File Name: test.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月15日 星期四 21时05分36秒
#########################################################################*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib>
int main()
{
    char *s1 = "this is a test";
    char *s2 = "test";

    int fd = open("test.txt", O_CREAT | O_TRUNC | O_RDWR); 
    int wr = write(fd, s1, lseek(fd, 0, SEEK_SET));
    int rd = read(fd, s2, lseek(fd, ))
    printf("%s\n%s\n", s2, s1);
    printf("%d\n%d\n", strlen(s2), strlen(s1));
    return 0;
}
