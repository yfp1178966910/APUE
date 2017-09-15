#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	int serverfifo = mkfifo("./server.fifo", O_CREAT | O_RDONLY | O_TRUNC);
	int n;
	if (serverfifo < 0 )
	{
		serverfifo = open("./server.fifo", O_RDONLY, 0777);
	} 

	char buf[1024];
	char *ptr;
	char fifoname[1024];
	while( (n = read(serverfifo, buf, 1024)) > 0)
	{
		if ( buf[n-1] == '\n' )
			buf[--n] = '\0';
		if ( (ptr = strchr(buf, ' ')) == NULL )
		{
			printf("bogus request: %s", buf);
			continue;
		}
		*ptr++ = 0;
		int wrfifo;
		snprintf(fifoname, sizeof(fifoname), "./fifo.%s", buf);
		if ( (wrfifo = open(fifoname, O_WRONLY, 0777)) < 0 )
		{
			printf("can not open : %s", fifoname);
			continue;
		}
		int fd;
		if ( (fd = open(ptr, O_RDONLY)) < 0 )
		{
			snprintf(buf+n, sizeof(buf)-n, "can not open: %s\n", strerror(errno));
		n = strlen(ptr);
		write(wrfifo, buf, n);
		close(wrfifo);
	
		}
		while( (n = read(fd, buf, 1024)) > 0)
			write(wrfifo, buf, n);
		close(wrfifo);
		close(fd);

	}
	return 0;
}
