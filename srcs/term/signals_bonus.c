#include <term/term.h>

t_term_err	term_interrupt(void)
{
	if (g_term.msg && (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) -1) == -1
	|| ((g_term.msg_len = ft_strlen(g_term.msg))
	&& (g_term.origin = strglen(g_term.msg))
	&& write(STDERR_FILENO, g_term.msg, g_term.msg_len) == -1)))
		return (TERM_EWRITE);
	if (g_term.has_caps)
		select_clear();
	if (g_term.line != g_term.hist.next)
	{
		line_clear(&g_term.line);
		g_term.hist.curr = g_term.hist.next;
		g_term.line = g_term.hist.next;
	}
	*g_term.line->data = '\0';
	g_term.line->len = 0;
	g_term.pos = 0;
	return (TERM_EOK);
}

t_term_err	term_eof(void)
{
	if (g_term.line->len == 0)
		return (TERM_EEOF);
	return (TERM_EOK);
}

t_term_err	term_stop(void)
{
	ft_dprintf(2, "[PROMPT][SIGNAL] STOP\n");
	return (TERM_EOK);
}

t_term_err	term_suspend(void)
{
	ft_dprintf(2, "[PROMPT][SIGNAL] SUSPEND\n");
	return (TERM_EOK);
}
