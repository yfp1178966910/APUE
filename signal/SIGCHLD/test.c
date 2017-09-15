#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>

void print_time()
{
	//char buf[1024] = {0};
	time_t t  = time(NULL);
	//sprintf(buf, "%s\n", t);
	printf("%ld\n", t);
}
	
void deamon()
{
	pid_t pid;
	pid = fork();
	if ( pid > 0 )
		exit(1);
	setsid();

	chdir("/");
	
	umask(0022);
	
	close(0);
	int fd = open("/dev/null", O_RDWR);
	dup2(fd, 1);
	dup2(fd, 2);
	

}

int main()
{
//	deamon();
	
	while( 1 )
	{
		//signal(SIGALRM, catch);
		//alarm(3);
		//pause();
		print_time();
	}
	return 0;
}
