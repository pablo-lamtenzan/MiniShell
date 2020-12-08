#include <term/term.h>

/*
**	Clear the screen's content, keeping only the current line.
*/
t_term_err	term_clear_screen()
{
	const t_pos	pos = g_term.caps.cursor.real;
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.ctrls.clear)
	{
		tputs(g_term.caps.ctrls.clear, 1, &putc_err);
		if ((!g_term.msg ||
		(status = term_origin(g_term.msg->data, g_term.msg->len)) == TERM_EOK))
			status = term_write(g_term.line->data, g_term.line->len);
		caps_goto(&g_term.caps, &pos);
	}
	return (status);
}

// TODO: Check if delete-mode needs to be activated before clearing parts

/*
**	Clear the screen from the cursor to the end of the screen.
*/
t_term_err	term_clear_eos()
{
	const int	x = g_term.caps.cursor.real.x;

	if (x)
	{
		tputs(g_term.caps.ctrls.del_eol, 1, &putc_err);
		if (x + g_term.line->len - g_term.caps.index >= (size_t)g_term.caps.width)
		{
			caps_goto_x(&g_term.caps, 0);
			caps_goto_y(&g_term.caps, g_term.caps.cursor.real.y + 1);
			tputs(g_term.caps.ctrls.del_eos, 1, &putc_err);
			caps_goto_y(&g_term.caps, g_term.caps.cursor.real.y - 1);
			caps_goto_x(&g_term.caps, x);
		}
	}
	else
		tputs(g_term.caps.ctrls.del_eos, 1, &putc_err);
	return (TERM_EOK);
}

// TODO: Alt-backspace

/*
**	Delete n characters from the terminal's input line, starting at the current
**	cursor position.
*/
t_term_err	term_line_del(size_t n)
{
	const t_pos		pos = g_term.caps.cursor.real;
	const int		index = g_term.caps.index;
	const size_t	remaining = g_term.line->len - n - index;
	t_term_err		status;

	status = TERM_EOK;
	if (line_erase(g_term.line, index, n))
	{
		if (pos.x + remaining < (size_t)g_term.caps.width)
			caps_delete(&g_term.caps, n);
		else
		{
			term_clear_eos();
			status = term_write(g_term.line->data + index, remaining);
			caps_goto(&g_term.caps, &pos);
			g_term.caps.index = index;
		}
	}
	return (status);
}

/*
**	Delete one character to the left of the cursor.
*/
t_term_err	term_backspace()
{
	if (g_term.caps.index > 0)
	{
		cursor_l();
		term_line_del(1);
	}
	return (TERM_EOK);
}

/*
**	Clear the current line visually, without deleting it's content.
*/
t_term_err	term_clear_line()
{
	caps_goto(&g_term.caps, &g_term.caps.cursor.origin);
	g_term.caps.index = 0;
	term_clear_eos();
	return (TERM_EOK);
}

/*
**	Write the line ending character.
*/
t_term_err	term_new_line()
{
	term_clear_line();
	if (write(STDERR_FILENO, g_term.line->data, g_term.line->len) == -1
	|| write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) - 1) == -1)
		return (TERM_EWRITE);
	if (g_term.line->len != 0)
	{
		//ft_dprintf(2, "[PROMPT] result: '%s'\n", g_term.line->data);
		if ((!g_term.caps.hist.next || g_term.line == g_term.caps.hist.next)
		&& !(g_term.caps.hist.next = line_new(10)))
			return (TERM_EALLOC);
		hist_add(&g_term.caps.hist, g_term.line);
	}
	return (TERM_ENL);
}
