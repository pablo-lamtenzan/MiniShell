#include <term/term.h>

t_term_err	term_interrupt(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) -1) == -1)
		return (TERM_EWRITE);
	if (!g_term.msg)
		g_term.caps.cursor.origin = g_term.caps.cursor.zero;
	else if ((status = term_origin(g_term.msg->data, g_term.msg->len)) != TERM_EOK)
		return (status);
	if (g_term.has_caps)
		select_clear();
	if (g_term.line != g_term.caps.hist.next)
	{
		line_clear(&g_term.line);
		g_term.caps.hist.curr = g_term.caps.hist.next;
		g_term.line = g_term.caps.hist.next;
	}
	*g_term.line->data = '\0';
	g_term.line->len = 0;
	g_term.caps.index = 0;
	return (status);
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
