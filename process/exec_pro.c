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
    
    printf("test ... pid == %d", getpid());

	// 子进程调用execlp， 并将dup2 修改文件描述符指向
	if ( pid == 0 )   
	{
		dup2(fd, STDOUT_FILENO);
		printf(".. \n\n\n\n\n child .. \n");   // not STDOUT_FILENO
		execlp("ps", "ps", "-a", NULL);  // STDOUT_FILENO to fd
	}

	char buf[1024] = {0};
	char *buf2 = "test buf2";  
    // char buf2[1024];

	// 父进程追加一段字符串
	
	if ( pid > 0 )
	{
		sleep(2);
		lseek(fd, 0, SEEK_END);
		int wr = write(fd, buf2, sizeof(buf2)); // write buf2 to fd 
		printf("%d %d\n", wr, lseek(fd, 0, SEEK_SET)); // 文件指针处于文件末尾, 
                                                        // lseek设置偏移，否则无法读取 
                                                                                            
        // lseek(fd, 0, SEEK_SET);
		int rd = read(fd, buf, sizeof(buf));
		printf("%d   %s\n", rd, buf);
	}

	return 0;

}
