/*
 *服务端：
 *1. 服务器不需要对文件进行截断操作
 *2. 服务端只需要读取映射区即可
 *3. 注意使用结构体类型指向映射区
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
	struct stu student;
	
	int fd = open("test.txt", O_RDWR);
	if ( fd == -1 )
	{
		printf("open error\n");
		exit(-1);
	}

	// 使用stu类型的指针执行已经建议的映射区
	struct stu  *m;
	// m = mmap(NULL, sizeof(student), PROT_READ, MAP_SHARED, fd, 0);
	m = mmap(NULL, 2*2*sizeof(student), PROT_READ, MAP_PRIVATE, fd, 0);
	close(fd);

	while(1)
	{
		printf("id = %d, name = %s, sex = %c\n", m->id, m->name, m->sex); // 这里取值是使用stu类型指针指向映射区数据
		sleep(2);
	}

	if ( munmap(m, sizeof(student)))
	{
		printf("munmap error\n");
		exit(-1);
	}

	return 0;
}
