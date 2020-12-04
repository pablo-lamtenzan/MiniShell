#include <term/term.h>

t_term_err	cursor_start_line(void)
{
	g_term.pos = 0;
	//ft_dprintf(2, "g_term.origin: %lu", g_term.origin);
	caps_goto(&g_term.caps, g_term.origin);
	return (TERM_EOK);
}

t_term_err	cursor_end_line(void)
{
	if (g_term.line && g_term.line->len)
	{
		g_term.pos = g_term.line->len;
		caps_goto(&g_term.caps, g_term.origin + g_term.pos);
	}
	return (TERM_EOK);
}

t_term_err	cursor_next_word(void)
{
	size_t	i;

	i = g_term.pos;
	while (i < g_term.line->len && ft_isspace(g_term.line->data[i]))
		i++;
	while (i < g_term.line->len && !ft_isspace(g_term.line->data[i]))
		i++;
	if (i != g_term.pos)
	{
		g_term.pos = i;
		caps_goto(&g_term.caps, g_term.origin + g_term.pos);
	}
	return (TERM_EOK);
}

t_term_err	cursor_prev_word(void)
{
	size_t	i;

	i = g_term.pos;
	while (i > 0 && ft_isspace(g_term.line->data[i - 1]))
		i--;
	while (i > 0 && !ft_isspace(g_term.line->data[i - 1]))
		i--;
	if (i != g_term.pos)
	{
		g_term.pos = i;
		caps_goto(&g_term.caps, g_term.origin + g_term.pos);
	}
	return (TERM_EOK);
}
