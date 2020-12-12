#include <term/term.h>

// TODO: ROW and COLUMNS environment variables

/*
**	Write the line ending character and add the current line to history.
*/
t_term_err	term_line_new(void)
{
	// TODO: Disable insertion and overwrite instead of clearing
	if (g_term.line->len) // if we have content
	{
		term_clear_line(); // clear the line visually
		if (write(STDERR_FILENO, g_term.line->data, g_term.line->len) == -1)
			return (TERM_EWRITE);
		else if ((!g_term.caps.hist.next || g_term.line == g_term.caps.hist.next) // if the next line does not exist or is the current line
		&& !(g_term.caps.hist.next = line_new(10))) // allocate a new line
			return (TERM_EALLOC);
		else
			hist_add(&g_term.caps.hist, g_term.line); // connect the current line
	}
	else if (g_term.caps.hist.curr != g_term.caps.hist.next)
	{
		line_clear(&g_term.line);
		g_term.caps.hist.curr = g_term.caps.hist.next;
		g_term.line = g_term.caps.hist.curr;
	}
	if (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) - 1) == -1)
		return(TERM_EWRITE);
	return (TERM_ENL);
}

// TODO: Alt-backspace
/*
**	Delete n characters from the terminal's input line, starting at the current
**	cursor position.
*/
t_term_err	term_line_del(size_t n)
{
	const t_pos		pos = g_term.caps.cursor.pos;
	const size_t	index = g_term.caps.index;
	const size_t	remaining = g_term.line->len - n - index;
	t_term_err		status;

	status = TERM_EOK;
	if (line_erase(g_term.line, index, n))
	{
		if (pos.x + remaining < (size_t)g_term.caps.width)
			caps_delete(&g_term.caps, n);
		else
		{
			// TODO: Toggle insert mode and delete overflow instead of eos
			term_clear_eos();
			status = term_write(g_term.line->data + index, remaining);
			caps_goto(&g_term.caps, pos);
			g_term.caps.index = index;
		}
	}
	return (status);
}

/*
**	Discard a line's content.
*/
t_term_err	term_line_discard(void)
{
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
	return (TERM_EOK);
}

/*
**	Kill a line's content on the left of the cursor.
*/
t_term_err	term_line_kill(void)
{
	const size_t	index = g_term.caps.index;

	caps_goto(&g_term.caps, g_term.caps.cursor.origin);
	g_term.caps.index = 0;
	term_line_del(index);
	return (TERM_EOK);
}
