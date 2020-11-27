#include "term.h"

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

bool	term_init_interactive(t_term *term, t_env **env)
{
	term->is_interactive = ft_isatty(STDIN_FILENO) && ft_isatty(STDERR_FILENO);
	if (term->is_interactive && !term_init_caps(term, env))
		ft_dprintf(2, "Failed to retrieve terminfo: %s\n", strerror(errno));
	return (true);
}

bool	term_init(t_term *term, t_env **env)
{
	ft_bzero(term, sizeof(*term));
	term->clip.select = (t_select){-1U, -1U};
	if (!(term->line = line_new(TERM_LINE_SIZE)))
		return (false);
	// TODO: Load and save history file
	term->line->prev = term->hist.last;
	*term->line->data = '\0';
	term->line->len = 0;
	term->hist.curr = term->line;
	term->hist.next = term->line;
	return (term_init_interactive(term, env));
}

void	term_destroy(t_term *term)
{
	hist_clear(&term->hist);
	if (term->hist.next != term->line)
		line_clear(&term->hist.next);
	line_clear(&term->line);
	clip_clear(term);
	if (term->has_caps)
		tcsetattr(STDIN_FILENO, TCSANOW, &term->caps.s_ios_orig);
}

/*
**	Prompt the user of an interactive terminal.
*/
t_term_err	term_prompt(t_term *term)
{
	//ft_dprintf(2, "%s%lu", term->msg, ft_strlen(term->msg));
	if (term->is_interactive && term->msg
	&& (term->msg_len = ft_strlen(term->msg))
	&& (term->origin = strglen(term->msg))
	&& write(STDERR_FILENO, term->msg, term->msg_len) == -1)
		return (TERM_EWRITE);
	if (term->has_caps)
		return (term_read_caps(term));
	return (term_read(term));
}
