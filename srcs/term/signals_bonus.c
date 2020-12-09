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
	term_line_discard();
	return (status);
}

t_term_err	term_eof(void)
{
	if (g_term.line->len == 0)
		return (TERM_EEOF);
	return (TERM_EOK);
}
