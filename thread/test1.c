#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <fcntl.h>

void myread(int fd[])
{
	char buf[1024];
	int ret = read(fd[0], buf, 1024);
	if ( ret < 0 )
		printf("read error");
	write(fd[1], buf, ret);
	printf("%s\n", buf);
}

int main(int argc, char *argv[])
{
	int fd[2];
	fd[0]= open(argv[1], O_RDONLY);
	fd[1]= open(argv[2], O_RDONLY);
	myread(fd);	
	int trd_num = atoi(argv[2]);
	printf("%d\n", trd_num);
	return 0;	
}




