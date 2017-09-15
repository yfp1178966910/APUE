/*
 *使用mmap实现IPC
 *其中，全局变量是独享的
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int var = 100;

void errno(char *str)
{
	printf("%s error\n", str);
	exit(-1);
}

int main()
{
	int fd = open("temp", O_CREAT | O_RDWR | O_TRUNC, 0664);
	if ( fd == -1 )
		errno("open");

	int *p;
	ftruncate(fd, 50);

	// 创建映射区
	p = (int *)mmap(NULL, 50, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if ( p == MAP_FAILED )
		errno("mmap");

	pid_t pid = fork();
	if ( pid == -1 )
		errno("fork");
	else if ( pid == 0 )  // 子进程
	{
		*p = 2000;
		var = 1000;
		printf(" child.. offset = %d .. var = %d\n", *p, var);
	}
	else
	{
		sleep(2);
		printf(" parent.. offset = %d .. var = %d\n", *p, var);
	}

	if ( munmap(p, 4) == -1 )
		errno("munmap");

	return 0;

}
