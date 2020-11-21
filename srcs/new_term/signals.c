#include "term.h"

t_term_err	term_interrupt(t_term *term)
{
	if (term->msg && (write(term->fds[2], TERM_ENDL, sizeof(TERM_ENDL) -1) == -1
	|| ((term->origin = ft_strlen(term->msg))
	&& write(term->fds[2], term->msg, term->origin) == -1)))
		return (TERM_EWRITE);
	if (term->has_caps)
		select_clear(term);
	if (term->line != term->hist.next)
	{
		line_clear(&term->line);
		term->hist.curr = term->hist.next;
		term->line = term->hist.next;
	}
	*term->line->data = '\0';
	term->line->len = 0;
	term->pos = 0;
	return (TERM_EOK);
}

t_term_err	term_eof(t_term *t)
{
	if (t->line->len == 0)
		return (TERM_EEOF);
	return (TERM_EOK);
}

t_term_err	term_stop(t_term *t)
{
	(void)t;
	ft_dprintf(2, "[PROMPT][SIGNAL] STOP\n");
	return (TERM_EOK);
}

t_term_err	term_suspend(t_term *t)
{
	(void)t;
	ft_dprintf(2, "[PROMPT][SIGNAL] SUSPEND\n");
	return (TERM_EOK);
}
