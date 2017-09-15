/*
 *客户端：
 *1。 客户端建立映射区
 *2.  对映射区内数据修改
 *3、 注意文件截断
 * 
 *
 *注意映射区权限
 * */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

struct stu 
{
	int id;
	char name[20];
	char sex;
};

int main()
{
	struct stu student = {1, "x", 'm'};
	
	int fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC);
	if ( fd == -1 )
	{
		printf("open error\n");
		exit(-1);
	}

	char *m;
	ftruncate(fd, sizeof(student));
	m = mmap(NULL, sizeof(student), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	close(fd);

	while(1)
	{
		memcpy(m, &student, sizeof(student));
		student.id++;
		sleep(2);
	}

	if ( munmap(m, sizeof(student)))
	{
		printf("munmap error\n");
		exit(-1);
	}

	return 0;
}
