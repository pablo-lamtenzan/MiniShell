#include "term.h"

#include <string.h>
#include <errno.h>

bool	term_init(t_term *term, t_env **env)
{
	ft_bzero(term, sizeof(*term));
	term->clip.select = (t_select){-1U, -1U};
	term->fds[0] = STDIN_FILENO;
	term->fds[1] = STDOUT_FILENO;
	term->fds[2] = STDERR_FILENO;
	if (!(term->line = line_new(TERM_LINE_SIZE)))
		return (false);
	// TODO: Load and save history file
	term->line->prev = term->hist.last;
	*term->line->data = '\0';
	term->line->len = 0;
	term->hist.curr = term->line;
	term->hist.next = term->line;
	if (!term_init_caps(term, env))
		ft_dprintf(2, "Failed to retrieve terminfo: %s\n", strerror(errno));
	return (true);
}

void	term_destroy(t_term *term)
{
	hist_clear(&term->hist);
	if (term->hist.next != term->line)
		line_clear(&term->hist.next);
	line_clear(&term->line);
	clip_clear(term);
	tcsetattr(term->fds[0], TCSANOW, &term->caps.s_ios_orig);
}

/*
**	Prompt the user of an interactive terminal.
*/
t_term_err	term_prompt(t_term *term)
{
	if (term->is_interactive && term->msg && (term->origin = ft_strlen(term->msg))
	&& write(term->fds[2], term->msg, term->origin) == -1)
		return (TERM_EWRITE);
	if (term->has_caps)
		return (term_read_caps(term));
	return (term_read(term));
}
