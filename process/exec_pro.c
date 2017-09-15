/*
 * 子进程调用execlp函数，将其输出的数据打印到文件中
 * */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	pid_t pid = fork();
	if ( pid == -1 )
	{
		printf("fork error\n");
		exit(1);
	}

	int fd = open("ps.out", O_CREAT | O_TRUNC | O_RDWR);


	// 子进程调用execlp， 并将dup2 修改文件描述符指向
	if ( pid == 0 )   
	{
		dup2(fd, STDOUT_FILENO);
		printf(".. \n\n\n\n\n child .. \n");   
		execlp("ps", "ps", "a", NULL);
	}

	char buf[100] = {0};
	char *buf2 = {"test buf2"};


	// 父进程追加一段字符串
	
	if ( pid > 0 )
	{
		sleep(2);
		lseek(fd, 0, SEEK_END);
		int wr = write(fd , buf2, sizeof(buf2));
		printf("%d\n", lseek(fd, 0, SEEK_SET));
		int rd = read(fd, buf, sizeof(buf2));
		printf("%d   %s\n", rd, buf);
	}

	return 0;

}
