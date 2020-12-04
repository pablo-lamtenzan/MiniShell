#include <term/term.h>
/*
**	Clear the screen's content, keeping only the current line.
*/
t_term_err	term_clear_screen()
{
	if (g_term.caps.mode.clear)
	{
		tputs(g_term.caps.mode.clear, 1, &putc_err);
		if ((g_term.msg
		&& write(STDERR_FILENO, g_term.msg, g_term.msg_len) == -1)
		|| write(1, g_term.line->data, g_term.line->len) == -1)
			return (TERM_EWRITE);
		g_term.pos = g_term.line->len;
	}
	return (TERM_EOK);
}

/*
**	Delete one character to the left of the cursor.
*/
t_term_err	term_backspace()
{
	if (g_term.pos > 0)
	{
		tputs(g_term.caps.ctrl.left, 1, &putc_err);
		caps_delete(&g_term.caps, 1);
		g_term.pos--;
		line_erase(g_term.line, g_term.pos, 1);
	}
	return (TERM_EOK);
}

/*
**	Clear the current line visually, without deleting it's content.
*/
t_term_err	term_clear_line()
{
	cursor_start_line();
	tputs(g_term.caps.ctrl.del_line, 1, &putc_err);
	return (TERM_EOK);
}

/*
**	Write the line ending character.
*/
t_term_err	term_new_line()
{
	if (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) - 1) == -1)
		return (TERM_EWRITE);
	if (g_term.line->len != 0)
	{
		//ft_dprintf(2, "[PROMPT] result: '%s'\n", g_term.line->data);
		if ((!g_term.hist.next || g_term.line == g_term.hist.next)
		&& !(g_term.hist.next = line_new(10)))
			return (TERM_EALLOC);
		hist_add(&g_term.hist, g_term.line);
	}
	return (TERM_ENL);
}
