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

/*
**	Detect and init the terminal's capabilities
**
**	returns true if successful, or false otherwise.
*/
bool		term_init_caps(t_env **env)
{
	const char	*term_type;
	char		term_buff[MAX_ENTRY + 1];
	int			ent_st;

	if (!env_set(env, "PS1", TERM_PS1, false)
	|| !env_set(env, "PS2", TERM_PS2, false)
	|| !(term_type = env_get(*env, "TERM", 4))
	|| (ent_st = tgetent(term_buff, term_type)) == -1)
		return (false);
	if (ent_st == 0)
		return (true);
	if (tcgetattr(STDIN_FILENO, &g_term.caps.s_ios) == -1)
		return (false);
	g_term.caps.s_ios_orig = g_term.caps.s_ios;
	g_term.caps.s_ios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG);
	g_term.caps.s_ios.c_cflag |= ONLCR;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.caps.s_ios) == -1)
		return (false);
	g_term.has_caps = caps_load(&g_term.caps);
	return (true);
}

bool	term_init(t_env **env)
{
	g_term.selec = (t_select){-1U, -1U};
	if (!(g_term.line = line_new(TERM_LINE_SIZE)))
		return (false);
	// TODO: Load and save history file
	g_term.line->prev = g_term.hist.last;
	*g_term.line->data = '\0';
	g_term.line->len = 0;
	g_term.hist.curr = g_term.line;
	g_term.hist.next = g_term.line;
	g_term.hist.head = g_term.line;
	g_term.is_interactive = ft_isatty(STDIN_FILENO) && ft_isatty(STDERR_FILENO);
	if (g_term.is_interactive && !term_init_caps(env))
		ft_dprintf(2, "Failed to retrieve terminfo: %s\n", strerror(errno));
	return (true);
}

void	term_destroy(void)
{
	if (g_term.hist.next != g_term.line)
		line_clear(&g_term.hist.next);
	if (g_term.hist.last != g_term.line)
		line_clear(&g_term.line);
	hist_clear(&g_term.hist);
	clip_clear();
	if (g_term.is_interactive)
		write(STDOUT_FILENO, TERM_EXIT, sizeof(TERM_EXIT) - 1);
}

/*
**	Prompt the user of an interactive terminal.
*/
t_term_err	term_prompt(const char **dst)
{
	t_term_err	status;

	if (g_term.is_interactive && g_term.msg
	&& (g_term.msg_len = ft_strlen(g_term.msg))
	&& (g_term.origin = strglen(g_term.msg))
	&& write(STDERR_FILENO, g_term.msg, g_term.msg_len) == -1)
		status = TERM_EWRITE;
	else if (g_term.has_caps)
		status = term_read_caps();
	else
		status = term_read();
	if (dst)
		*dst = g_term.line->data;
	return(status);
}
