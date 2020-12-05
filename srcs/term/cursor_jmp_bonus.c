#include <term/term.h>

t_term_err	cursor_goto_index(size_t index)
{
	t_pos	pos;

	if (index != g_term.caps.index)
	{
		g_term.caps.index = index;
		g_term.caps.cursor.pos.x = index % g_term.caps.width;
		g_term.caps.cursor.pos.y = index / g_term.caps.width;
		//ft_dprintf(2, "%d, %d;", g_term.caps.cursor.pos.x, g_term.caps.cursor.pos.y);
		pos = (t_pos) {
			g_term.caps.cursor.origin.x + g_term.caps.cursor.pos.x,
			g_term.caps.cursor.origin.y + g_term.caps.cursor.pos.y,
		};
		caps_goto(&g_term.caps, &pos);
	}
	return (TERM_EOK);
}

/*
**	Place the cursor at the origin.
**
**	Updates the cursor's position and real position.
*/
t_term_err	cursor_start_line(void)
{
	return (cursor_goto_index(0));
}

t_term_err	cursor_end_line(void)
{
	return (cursor_goto_index(g_term.line->len));
}

t_term_err	cursor_next_word(void)
{
	size_t	i;

	i = g_term.caps.index;
	while (i < g_term.line->len && ft_isspace(g_term.line->data[i]))
		i++;
	while (i < g_term.line->len && !ft_isspace(g_term.line->data[i]))
		i++;
	if (i != g_term.caps.index)
		cursor_goto_index(i);
	return (TERM_EOK);
}

t_term_err	cursor_prev_word(void)
{
	size_t	i;

	i = g_term.caps.index;
	while (i > 0 && ft_isspace(g_term.line->data[i - 1]))
		i--;
	while (i > 0 && !ft_isspace(g_term.line->data[i - 1]))
		i--;
	if (i != g_term.caps.index)
		cursor_goto_index(i);
	return (TERM_EOK);
}
