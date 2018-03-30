#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void do_sig(int signo)
{
	pid_t pid;
	int status;
	while ( (pid = waitpid(0, NULL, WNOHANG)) > 0 )
	{
		if (WIFEXITED(status))
			printf("-------child %d exit %d\n", pid, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			printf("-------child %d exit %d\n", pid, WTERMSIG(status));
	}
	
}

int main()
{

	pid_t pid;
	int fd[2];
	
	pipe(fd);
	pid = fork();
	
	// 子进程关闭读端，打开写端。以标准输出的方式将execlp的数据写到管道内 	
	if ( pid == 0 )
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execlp("ls", "ls", "-a", NULL);
	}
	// 父进程关闭写端，打开读端，以标准输入的方式将管道内的数据读出
	else if ( pid > 0 ) 
	{
		signal(SIGCHLD, do_sig); // 捕捉SIGCHLD信号
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		execlp("wc", "wc", "-l", NULL);
	}
	return 0;
}	
