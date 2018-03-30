/*#########################################################################
# File Name: sigpending.c
# Author: yanfeipeng
# address: seclab
# Created Time: 2018年03月28日 星期三 22时40分38秒
#########################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void err_sys(char *s)
{
    printf("%s error\n", s);
    exit(1);
}

void sig_quit()
{
    printf("sig catch\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
        err_sys("reset sigquit");
}

int main()
{
    sigset_t newmask, oldmask, pendmask;

    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        err_sys("can't catch");

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK");

    sleep(5);

    if (sigpending(&pendmask) < 0)
        err_sys("sigpending");
    if (sigismember(&pendmask, SIGQUIT))
        printf("SIGOUIT pending\n");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK");
    printf("SIGQUIT unblocked\n");

    sleep(5);
    exit(0);
}
