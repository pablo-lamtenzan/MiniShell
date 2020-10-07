// EXEC
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

// TYPES
#include <stdbool.h>

// PRINT
#include <stdio.h>

// OPEN
#include <fcntl.h>

// ERRNO
#include <string.h>
#include <errno.h>

# define STDIO_CT	3

# define REDIR_LE	1
# define REDIR_GR	2
# define REDIR_DG	4

# define REDIR		REDIR_LE | REDIR_GR | REDIR_DG

# define O_REDIR_LE	O_RDONLY
# define O_REDIR_GR	O_WRONLY | O_CREAT | O_TRUNC
# define O_REDIR_DG	O_WRONLY | O_CREAT | O_APPEND

# define UMASK		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int		close_files(int *fds)
{
	int i;

	i = 0;
	while (i < STDIO_CT)
	{
		if (fds[i] != i)
		{
			dprintf(2, "[redir] closing %d\n", fds[i]);
			if (close(fds[i]) < 0)
			{
				dprintf(2, "[redir] dup2 returned '-1'!\n");
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
				return (-1);
			}
		}
		i++;
	}
	return (1);
}

int		redir_file(int fds[STDIO_CT], int redir_type, const char *filepath)
{
	fds[0] = 0;
	fds[1] = 1;
	fds[2] = 2;
	if (!(redir_type & REDIR))
		return (0);
	if (redir_type & REDIR_LE)
		fds[0] = open(filepath, O_REDIR_LE, UMASK);
	if (redir_type & REDIR_GR)
		fds[1] = open(filepath, O_REDIR_GR, UMASK);
	if (redir_type & REDIR_DG)
		fds[1] = open(filepath, O_REDIR_DG, UMASK);
	if (fds[0] < 0 || fds[1] < 0)
	{
		dprintf(2, "[redir] error: fds[0] = %d, fds[1] = %d!\n", fds[0], fds[1]);
		dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
		return (-1);
	}
	dprintf(2, "[redir][opened] '%s'!\n", filepath);
	dprintf(2, "[redir] fds[0] = %d, fds[1] = %d!\n", fds[0], fds[1]);
	return (1);
}

int		dup_stdio(int fds[STDIO_CT])
{
	int i;

	i = 0;
	while (i < STDIO_CT)
	{
		if (fds[i] != i)
		{
			dprintf(2, "[redir][dup] %d -> %d\n", fds[i], i);
			if (dup2(fds[i], i) < 0)
			{
				dprintf(2, "[redir][dup][error] dup2 returned '-1'!\n");
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
				return (-1);
			}
			dprintf(2, "[redir][close] fork fd '%d'...\n", fds[i]);
			if (close(fds[i]) < 0)
			{
				dprintf(2, "[redir][close][error] close(%d) returned -1!\n", fds[i]);
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
				return (-1);
			}
		}
		i++;
	}
	return (1);
}

bool	exec_and_join(const char *binary, char* const *av, char *const *ep, int *status, int *fds)
{
	pid_t	pid;
	int		ret;

	if (!(pid = fork()))
	{
		dprintf(2, "[exec][child] executing '%s'...\n", binary);
		dup_stdio(fds);
		// *(int*)0=0; // Use this to simulate SEGFAULT
		ret = execve(binary, av, ep);
		dprintf(2, "[exec][child] execve returned '%d'!\n", ret);
		dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
		exit(ret);
	}
	else if (pid < 0)
	{
		dprintf(2, "[exec][parent] error: pid is '%d'!\n", pid);
		dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
		return (false);
	}
	dprintf(2, "[exec][parent] joining child with pid '%d'!\n", pid);
	while (!waitpid(pid, status, 0))
		;
	dprintf(2, "[exec][parent] joined child!\n");
	return (true);
}

int		handle_status(int status)
{
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		dprintf(2, "[exec][status] child exited with status '%d'!\n", status);
		return (status);
	}
	if (WIFSIGNALED(status))
	{
		status = WTERMSIG(status);
		dprintf(2, "[exec][status] %s\n", strsignal(status));
		return (status);
	}
	dprintf(2, "[exec][status] cannot retrieve child status!\n");
	return (status);
}

int	example_simple_redir(const char *binary, char *const *av, char *const *ep)
{
	int	status;
	int	fds[STDIO_CT];

	status = -1;
	return (redir_file(fds, REDIR_LE, "redir.log") != -1
		&& exec_and_join(binary, av, ep, &status, fds)
		&& close_files(fds)
		&& handle_status(status)
	);
}

int	example_pipe(const char *binary[3], char *av[3][3], char *ep[3][1])
{
	static const int	count = 2;
	int					i;
	int					pipe_fds[2];
	int					fds[STDIO_CT];
	int					status;

	i = 0;
	while (i < count)
	{
		if (i % 2 == 0)
		{
			if (pipe(pipe_fds) < 0)
			{
				dprintf(2, "[pipe][error] pipe returned '-1'!\n");
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
				return (-1);
			}
			dprintf(2, "[pipe] pipe_fds[0] = %d, pipe_fds[1] = %d\n", pipe_fds[0], pipe_fds[1]);
			fds[0] = 0;
			fds[1] = pipe_fds[1];
			fds[2] = 2;
		}
		else
		{
			dprintf(2, "[pipe][close] pipe_fds[1] = '%d'...\n", pipe_fds[1]);
			if (close(pipe_fds[1]) < 0)
			{
				dprintf(2, "[pipe][close] returned '-1'\n!");
				dprintf(2, "[errno] %d: %s\n", errno, strerror(errno));
			}
			fds[0] = pipe_fds[0];
			fds[1] = 1;
			fds[2] = 2;
		}
		exec_and_join(binary[i], av[i], ep[i], &status, fds);
		i++;
	}
	return (0);
}

int	main(void)
{
	static const char	*bins[3] = {"/bin/ls", "/bin/cat", "/usr/bin/rev"};
	char				*avs[3][3] = {{"ls", NULL}, {"cat", "-e", NULL}, {"rev", NULL}};
	char				*eps[3][1] = { {NULL} };

	example_pipe(bins, avs, eps);
//	example_simple_redir(bins[0], avs[0], eps[0]);
	return (0);
} 
