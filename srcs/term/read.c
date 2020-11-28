#include <term/term.h>

/*
**	Control key-strokes.
*/
t_term_err	term_cntrl(char c)
{
	static const t_keybind	keys[] =
	{
		{CINTR, &term_interrupt},
		{CERASE, &term_backspace},
		{CEOF, &term_eof},
		{CSTOP, &term_stop},
		{CSUSP, &term_suspend},
//		{'u' - TERM_CNTRL, &term_clear_line},
		{'h' - TERM_CNTRL, &term_backspace},
		{'l' - TERM_CNTRL, &term_clear_screen},
		{'j' - TERM_CNTRL, &term_new_line},
		{'a' - TERM_CNTRL, &cursor_start_line},
		{'e' - TERM_CNTRL, &cursor_end_line},
		{'y' - TERM_CNTRL, &clip_paste},
		{'k' - TERM_CNTRL, &clip_cut},
		{'p' - TERM_CNTRL, &term_prev_line},
		{'n' - TERM_CNTRL, &term_next_line}
	};
	t_term_action	action;

	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}

/*
**	Read and parse a terminal's input.
*/
int		term_read()
{
	t_term_err	status;
	int			read_st;

	status = TERM_EOK;
	free(g_term.line->data);
	g_term.line->data = NULL;
	g_term.line->len = 0;
	g_term.line->size = 0;
	if ((read_st = get_next_line(STDIN_FILENO, &g_term.line->data)) == -1)
		return (TERM_EREAD);
	g_term.line->len = ft_strlen(g_term.line->data);
	g_term.line->size = g_term.line->len + 1;
	if (read_st == 1 || (read_st == 0 && g_term.line->len != 0))
	{
		ft_dprintf(2, "[PROMPT][GNL][%3lu] '%s'\n", g_term.line->len, g_term.line->data);
		return (TERM_ENL);
	}
	return (TERM_EEOF);
}

t_term_err	term_read_echo(char c)
{
	t_term_err	status;

	status = TERM_EOK;
	if (!line_insert(g_term.line, g_term.pos, &c, 1))
		status = TERM_EALLOC;
	else if (write(STDERR_FILENO, &c, 1) == -1)
		status = TERM_EWRITE;
	else
		g_term.pos++;
	return (status);
}

/* 
int			ft_isutf8(unsigned char c)
{
	return (c >= 128);
}

t_term_err	term_read_utf8(char c)
{
	t_term_err	status;

	status = TERM_EOK;

	return (status);
}
*/

/*
**	Read and parse an interactive terminal's input.
*/
t_term_err	term_read_caps(void)
{
	t_term_err	status;
	ssize_t		read_st;
	char		c;

	//ft_dprintf(2, "origin: %lu", g_term.origin);
	status = TERM_EOK;
	g_term.line = g_term.hist.next;
	g_term.pos = 0;
	tputs(g_term.caps.mode.insert, 1, ft_putchar);
	while (status == TERM_EOK)
	{
		if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
			status = (read_st == 0) ? TERM_EEOF : TERM_EREAD;
		else if (c == TERM_ESC)
			status = term_read_esc();
		else if (ft_iscntrl(c))
			status = term_cntrl(c);
		else if (ft_isascii(c))
			status = term_read_echo(c);
	}
	tputs(g_term.caps.mode.insert_end, 1, ft_putchar);
	return (status);
}
