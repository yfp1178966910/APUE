/*
 *通过SIGCHLD信号，回收子进程
 *
 *
 * */



#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

void do_sig_child(int signo)
{
	int status;
	pid_t pid;
	// 这里使用wihle， 是考虑同时会有子进程死亡，而未决信号集只记录一次，如果使用if 那么有可能会有些子进程死亡的时候变成僵尸进程，没有被回收掉。采用轮巡的方式回收，是为了防止少回收。
	while ((pid = waitpid(0, &status, WNOHANG)) > 0)
	{
		if (WIFEXITED(status))
			printf("----------child %d exit %d\n", pid, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			printf("-----------child %d exit %d\n", pid, WTERMSIG(status));
	}
		
}

int main()
{
	int i;
	pid_t pid;	
	// 创建10个子进程
	for(i=0; i<10; i++)
	{
		pid = fork();
		if( pid == 0)
		{
			break;
		}
		else if ( pid < 0)
		{
			printf("fork error\n");
			exit(0);
		}
	}
	
	if (pid == 0)    // 子进程
	{
		int n = 1;
		while(n--)
		{
			printf("child ID = %d\n", getpid());
		}
		return i+1;
	}
	else if ( pid > 0 )
	{
		// 父进程，捕捉SIGCHLD信号
		struct sigaction act, oldact;
		act.sa_flags = 0;
		sigemptyset(&act.sa_mask);
		act.sa_handler = do_sig_child;
		
		sigaction(SIGCHLD, &act, &oldact);
		
		sleep(3);
		while(1)
		{
			printf("parent ID = %d\n", getpid());
			sleep(1);
		}
	}

	return 0;
}
