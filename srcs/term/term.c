#include <term/term.h>

#include <fcntl.h>
#include <string.h>
#include <errno.h>

int		ft_isatty(int fd)
{
	struct stat	st;
	struct stat	null_st;
	int			null_fd;
	int			ret;

	ret = 0;
	if (fstat(fd, &st))
		ft_dprintf(STDERR_FILENO, "Could not stat fd %d: %s\n",
			fd, strerror(errno));
	else if (S_ISCHR(st.st_mode))
	{
		if ((null_fd = open(TERM_DEV_NULL, O_RDONLY)) == -1)
			ft_dprintf(STDERR_FILENO, "Could not open "TERM_DEV_NULL": %s",
				strerror(errno));
		else
		{
			if (fstat(null_fd, &null_st))
				ft_dprintf(STDERR_FILENO, "Could not stat fd %d: %s\n",
					null_fd, strerror(errno));
			else
				ret = st.st_ino != null_st.st_ino;
			close(null_fd);
		}
	}
	return (ret);
}

// TODO: Add dirname to signature and environment
bool	term_init(t_env **env)
{
	if (!(g_term.line = line_new(TERM_LINE_SIZE)))
		return (false);
	g_term.is_interactive = ft_isatty(STDIN_FILENO) && ft_isatty(STDERR_FILENO);
	if (g_term.is_interactive && !(env_set(env, "PS1", TERM_PS1, false)
		&& env_set(env, "PS2", TERM_PS2, false)))
		ft_dprintf(2, "Failed to retrieve terminfo: %s\n", strerror(errno));
	return (true);
}

void	term_destroy(void)
{
	if (g_term.caps.hist.next != g_term.line)
		line_clear(&g_term.caps.hist.next);
	if (g_term.caps.hist.last != g_term.line)
		line_clear(&g_term.line);
	line_clear(&g_term.msg);
	if (g_term.is_interactive)
		write(STDOUT_FILENO, TERM_EXIT, sizeof(TERM_EXIT) - 1);
}

/*
**	Prompt the user of an interactive terminal.
*/
t_term_err	term_prompt(const char **dst)
{
	t_term_err	status;

	if (g_term.is_interactive && g_term.msg->len
	&& write(STDERR_FILENO, g_term.msg->data, g_term.msg->len) == -1)
		status = TERM_EWRITE;
	else
		status = term_read();
	if (dst)
		*dst = g_term.line->data;
	return(status);
}
