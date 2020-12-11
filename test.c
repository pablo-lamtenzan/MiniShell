

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
	int	i;

	i = 0;
	(void)argc;
	(void)argv;
	write(1, "------------------------------\n", 31);
	while (envp[i])
	{
		write(1, envp[i], strlen(envp[i]));
		write(1, "\n", 1);
		i++;
	}
	write(1, "------------------------------\n", 31);
	printf("TOTAL ENVIRONMENT VARIABLES: %d\n", i);
	return (0);
}