#include <term/term.h>
/*
**	Clear the screen's content, keeping only the current line.
*/
t_term_err	term_clear_screen()
{
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.ctrls.clear)
	{
		tputs(g_term.caps.ctrls.clear, 1, &putc_err);
		if ((!g_term.msg ||
		(status = term_origin(g_term.msg->data, g_term.msg->len)) == TERM_EOK))
			status = term_write(g_term.line->data, g_term.line->len);
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
		caps_delete(&g_term.caps, 1);
		line_erase(g_term.line, g_term.caps.index, 1);
	}
	return (TERM_EOK);
}

/*
**	Clear the current line visually, without deleting it's content.
*/
t_term_err	term_clear_line()
{
	cursor_start_line();
	tputs(g_term.caps.ctrls.del_line, 1, &putc_err);
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
		if ((!g_term.caps.hist.next || g_term.line == g_term.caps.hist.next)
		&& !(g_term.caps.hist.next = line_new(10)))
			return (TERM_EALLOC);
		hist_add(&g_term.caps.hist, g_term.line);
	}
	return (TERM_ENL);
}
