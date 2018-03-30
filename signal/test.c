/*#########################################################################
# File Name: test.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月28日 星期三 22时37分02秒
#########################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

void sic_catch()
{
    printf("catch\n");
}


int main()
{
    if( signal(SIGQUIT, sic_catch) == SIG_ERR)
        printf("can't catch sigquit\n'");

    while(1)
    {
        printf("this is a test\n");
        sleep(2);
    }

    return 0;
}
