#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	int fd;
	struct flock f_lock;

	if ( argc < 2 )
	{
		printf("ERROR \n");
		exit(1);
	}

	if (( fd = open(argv[1], O_RDWR)) < 0 )
	{
		printf("open error\n");
		exit(1);
	}

	f_lock.l_type = F_WRLCK;  // 写锁
	//f_lock.l_type = F_RDLCK； // 读锁

	f_lock.l_whence = SEEK_SET;
	f_lock.l_start = 0;
	f_lock.l_len = 0;

	fcntl(fd, F_SETLKW, &f_lock);
	printf("get lock\n");

	sleep(10);

	f_lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLKW, &f_lock);
	printf("un lock\n");

	close(fd);
	
}

