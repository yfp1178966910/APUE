#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main()
{
	char fifoname[1024];
	char buf[1024];
	int pid = getpid();
	snprintf(fifoname, sizeof(fifoname), "./fifo.%ld", (long)pid);
	if ( mkfifo(fifoname, O_RDONLY) < 0 )
	{
		printf("can't creat %s", fifoname);
	}
	
	snprintf(buf, sizeof(buf), "%ld", (long)pid);
	int len = strlen(buf);
	char *ptr;
	ptr = buf + len;
	fgets(ptr, 1024-len, stdin);
	len = strlen(buf);
	
	int writefifo = open("./server.fifo", O_WRONLY);
	write(writefifo, buf, len);

	int readfifo = open(fifoname, O_RDONLY);
	int n;
	while ( (n = read(readfifo, buf, 1024)) > 0 )
	{
		write(STDOUT_FILENO, buf, n);
	}
	
	close(readfifo);
	unlink(fifoname);
	
	return 0;
}
