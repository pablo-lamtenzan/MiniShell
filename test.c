

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int	main(int argc, char **argv, char **envp)
{
	kill(getpid(), SIGSEGV);
	return (123);
}