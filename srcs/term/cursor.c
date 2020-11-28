#include <term/term.h>

t_term_err	cursor_l(void)
{
	if (g_term.pos > 0)
	{
		g_term.pos--;
		tputs(g_term.caps.ctrl.left, 1, &putc_err);
	}
	return (TERM_EOK);
}

t_term_err	cursor_r(void)
{
	if (g_term.pos < g_term.line->len)
	{
		g_term.pos++;
		tputs(g_term.caps.ctrl.right, 1, &putc_err);
	}
	return (TERM_EOK);
}
