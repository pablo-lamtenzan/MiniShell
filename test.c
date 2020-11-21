

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	/*
	pid_t	pid = fork();
	int fd = open("/dev/null", O_RDONLY);

	if (pid == 0)
	{
		if (fd)
		{
			dup2(fd, 1);
			close(fd);
			char *args[]={"pidof","./a.out"}; 
        	execvp(args[0],args); 
			write(2, "FAIL\n", 6);
			exit(1);
		}
	}
	else if (pid)
	{
		char	buff[512];
		int		bytes;

		wait(NULL);
		while ((bytes = read(fd, &buff, 512) > 0))
		{
			write(2, buff, bytes);
		}
		printf("END WRITE\n");
	}
	*/
    int pid = getpid();
    dprintf(2, "Printf from test.c %d\n", pid);
    kill(pid, SIGSTOP);
	//sleep(20);
    dprintf(2, "TEST\n");
    kill(pid, SIGSTOP);
    dprintf(2, "TEST2\n");
    return (0);
}