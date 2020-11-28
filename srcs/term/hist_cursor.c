#include <term/term.h>

// TODO: Use line_insert instead of freeing line
t_term_err	term_prev_line(void)
{
	//ft_dprintf(2, "origin: %lu", g_term.origin);
	if (g_term.hist.curr->prev)
	{
		if (g_term.hist.curr != g_term.hist.next)
		{
			hist_commit(&g_term.hist, g_term.line);
			free(g_term.line);
		}
		g_term.hist.curr = g_term.hist.curr->prev;
		term_clear_line();
		if (write(1, g_term.hist.curr->data, g_term.hist.curr->len) == -1)
			return (TERM_EWRITE);
		if (!(g_term.line = line_dup(g_term.hist.curr)))
			return (TERM_EALLOC);
		g_term.pos = g_term.hist.curr->len;
	}
	return (TERM_EOK);
}

t_term_err	term_next_line(void)
{
	if (g_term.hist.curr->next)
	{
		if (g_term.hist.curr != g_term.hist.next)
		{
			hist_commit(&g_term.hist, g_term.line);
			free(g_term.line);
		}
		g_term.hist.curr = g_term.hist.curr->next;
		term_clear_line();
		if (write(1, g_term.hist.curr->data, g_term.hist.curr->len) == -1)
			return (TERM_EWRITE);
		if (g_term.hist.curr == g_term.hist.next)
			g_term.line = g_term.hist.curr;
		else if (!(g_term.line = line_dup(g_term.hist.curr)))
			return (TERM_EALLOC);
		g_term.pos = g_term.hist.curr->len;
	}
	return (TERM_EOK);
}
