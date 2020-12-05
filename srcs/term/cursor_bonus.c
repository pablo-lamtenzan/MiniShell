#include <term/term.h>

t_term_err	cursor_l(void)
{
	if (g_term.caps.index > 0)
		return (cursor_goto_index(g_term.caps.index - 1));
	return (TERM_EOK);
}

t_term_err	cursor_r(void)
{
	// nw -> go forward one line (clearing rest of line)
	if (g_term.caps.index < g_term.line->len)
		return (cursor_goto_index(g_term.caps.index + 1));
	return (TERM_EOK);
}
