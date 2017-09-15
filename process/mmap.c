/*
 *mmap创建映射区
 1. 不能创建大小为0的映射区
 2. 权限问题需要注意：
	a. 映射区文件的权限 <= 文化
	b. 创建映射区的过程隐含了一次读操作，至少需要读权限
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	int fd = open("test.txt", O_CREAT | O_RDWR);
	if ( fd < 0 )
	{
		perror("open error\n");
		exit(1);
	}
	char *p = NULL; // char *类型 指向 mmap

	int len = ftruncate(fd, 4); // 将 fd 截断为4个字节
	p = mmap(NULL, 4, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if ( p == MAP_FAILED )
	{
		perror("mmap error\n");
		exit(1);
	}
	strcpy(p, "abc");
	close(fd);

	if ( munmap(p, 4) == -1 ) // 释放
	{
		perror("munmap error\n");
		exit(1);
	}

	return 0;

}
