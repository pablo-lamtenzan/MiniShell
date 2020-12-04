#include <term/term.h>


/*
**	Control key-strokes.
*/
t_term_err	term_cntrl(char c)
{
	const t_keybind	keys[] =
	{
		{g_term.caps.s_ios.c_cc[VINTR], &term_interrupt},
		{g_term.caps.s_ios.c_cc[VERASE], &term_backspace},
		{g_term.caps.s_ios.c_cc[VEOF], &term_eof},
		{g_term.caps.s_ios.c_cc[VSTOP], &term_stop},
		{g_term.caps.s_ios.c_cc[VSUSP], &term_suspend},
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

t_term_err	term_read_echo(char c)
{
	t_term_err	status;

	status = TERM_EOK;
	select_clear();
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

	status = TERM_EOK;
	g_term.line = g_term.hist.next;
	g_term.pos = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.caps.s_ios) == -1)
		return (TERM_ESETATTR);
	tputs(g_term.caps.mode.insert, 1, &putc_err);
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
	tputs(g_term.caps.mode.insert_end, 1,  &putc_err);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.caps.s_ios_orig) == -1
	&& status >= 0)
		status = TERM_ESETATTR;
	return (status);
}
