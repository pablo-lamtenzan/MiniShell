#include <term/term.h>

t_term_err	term_set_line(t_line *line)
{
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.hist.curr != g_term.caps.hist.next)
	{
		hist_commit(&g_term.caps.hist, g_term.line);
		free(g_term.line);
	}
	g_term.caps.hist.curr = line;
	if (!(g_term.line = line_dup(line)))
		status = TERM_EALLOC;
	else
	{
		term_clear_line();
		status = term_write(g_term.line->data, g_term.line->len);
	}
	return (status);
}

t_term_err	term_prev_line(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.hist.curr->prev)
		status = term_set_line(g_term.caps.hist.curr->prev);
	return (status);
}

t_term_err	term_next_line(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.hist.curr->next)
		status = term_set_line(g_term.caps.hist.curr->next);
	return (status);
}
