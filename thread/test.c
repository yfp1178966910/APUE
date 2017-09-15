#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

# define MAX 20

typedef struct file {
	int old;
	int new;
	int off;
	int len;
}file;

void myerror(char *s)
{
	printf("%s error\n", s);
	exit(0);
}

void *fuc(void *arg)
{
	printf("thread start\n***************************\n");
	file *file_arg = (file *)arg;
	lseek(file_arg->new, file_arg->off, SEEK_SET);
	int start = lseek(file_arg->old, file_arg->off, SEEK_SET);
	char buf[1024];

	int num = file_arg->len / 1024;
	int num_fin = file_arg->len % 1024;
	int i, ret;

	for ( i=0; i<=num; i++) 
	{
		if ( i != num )
			ret = read(file_arg->old, buf, 1024);
		else
			ret = read(file_arg->old, buf, num_fin);

		write(file_arg->new, buf, ret);
		memset(buf, 0, 1024);
		usleep(10);
	}
	
	pthread_exit((void *)0);	
	
}

int main(int argc, char *argv[])
{
	
	int fd1 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC);
	int fd = open(argv[1], O_RDONLY, 0777);

	int len = lseek(fd, 0, SEEK_END);
	int thread_num = atoi(argv[2]);
	int len_avg = len / thread_num;
	int len_fin = len % thread_num;
	
	file *file_arg = malloc(sizeof(file));
	file_arg->old = fd;
	file_arg->new = fd1;

	int i;
	
	pthread_t tid[20];
	
	for( i=0; i<thread_num; i++)
	{
		file_arg->off = i * len_avg;
		if ( i < thread_num - 1 )
			file_arg->len = len_avg;
		else
			file_arg->len = len_fin;

		pthread_create(&tid[i], NULL, fuc, (void *)file_arg);
	}

	pthread_exit((void *)0);
}
