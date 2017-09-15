#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>

// 捕捉函数
void print_time(int signo)
{
	int fd = open("date.log", O_CREAT | O_RDWR | O_APPEND);
	char buf[100];
	time_t t = time(NULL);
	if (t == -1)
	{
		exit(1);
	}
	sprintf(buf, "time is %ld\n", t);
	write(fd, buf, strlen(buf));
	memset(buf, 0, 100);
	close(fd);	
}

// 自定义计时器
void mysleep(int seconds)
{
	if (signal(SIGALRM, print_time) == SIG_ERR)	
		exit(1);
	//alarm(seconds);
	
	struct itimerval it, oldit;
	it.it_value.tv_sec = seconds;
	it.it_value.tv_usec = 0;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_usec = 0;

	
	if (setitimer(ITIMER_REAL, &it, &oldit))
	{
		perror("setitimer error\n");
		exit(0);
	}
	pause();
	
	
}
void mydaemond_init()
{
	// 1. 创建子进程，关闭父进程
	pid_t pid = fork();
	if ( pid > 0 )
		exit(1);
	
	// 2. 创建会话，以当前进程号为会长，组长，丢弃终端
	pid_t spid = setsid();
	if ( spid == -1 )
	{
		perror(" setsid error\n");
		exit(1);
	}
	
	// 3. 改变当面目录为根目录
	int ret = chdir("/");
	if ( ret == -1 )
	{
		perror(" chdir error\n");
		exit(1);
	}
	
	// 4. 重设文件权限掩码， 防止有些文件创建屏蔽而拒绝某些权限，增加守护进程的灵活性
	umask(0022);

	// 5. 关闭文件描述符，0,1,2， 浪费系统资源，无法卸载
	close(STDIN_FILENO);
	int fd = open("/dev/null", O_RDWR);
	//dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	
}

int main()
{
	mydaemond_init();
	//int fd = open("data.log", O_CREAT | O_RDWR | O_APPEND, 0664);
	while ( 1 )
	{
		mysleep(5);
	}
	return 0;
		
}
