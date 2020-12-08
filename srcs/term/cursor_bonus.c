#include <term/term.h>

t_term_err	cursor_l(void)
{
	if (g_term.caps.index > 0)
		return (cursor_goto_index(g_term.caps.index - 1));
	return (TERM_EOK);
}

t_term_err	cursor_r(void)
{
	if (g_term.caps.index < g_term.line->len)

t_term_err	cursor_d(void)
{	
	if (g_term.caps.index < g_term.line->len)
		tputs(g_term.caps.ctrls.down, 1, &putc_err);
	else
		tputs(g_term.caps.ctrls.scroll_down, 1, &putc_err);
	return (TERM_EOK);
}
