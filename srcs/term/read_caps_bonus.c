#include <term/term.h>


// TODO: Put keybinds in static variable at initialization
/*
**	Control key-strokes.
*/
static t_term_err	term_cntrl(char c)
{
	const t_keybind	keys[] =
	{
		{g_term.caps.s_ios.c_cc[VINTR], &term_interrupt},
		{g_term.caps.s_ios.c_cc[VERASE], &term_backspace},
		{g_term.caps.s_ios.c_cc[VEOF], &term_eof},
		{g_term.caps.s_ios.c_cc[VSTOP], &term_stop},
		{g_term.caps.s_ios.c_cc[VSUSP], &term_suspend},
//		{'u' - TERM_CNTRL, &term_clear_line},
		{'h' - ANSI_CNTRL, &term_backspace},
		{'l' - ANSI_CNTRL, &term_clear_screen},
		{'j' - ANSI_CNTRL, &term_new_line},
		{'a' - ANSI_CNTRL, &cursor_start_line},
		{'e' - ANSI_CNTRL, &cursor_end_line},
		{'y' - ANSI_CNTRL, &clip_paste},
		{'k' - ANSI_CNTRL, &clip_cut},
		{'p' - ANSI_CNTRL, &term_prev_line},
		{'n' - ANSI_CNTRL, &term_next_line}
	};
	t_term_action	action;

	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}

static t_term_err	term_read_echo(char c)
{
	t_term_err	status;

	status = TERM_EOK;
	select_clear();
	if (!line_insert(g_term.line, g_term.caps.index, &c, 1))
		status = TERM_EALLOC;
	else
		status = term_write(&c, 1);
	return (status);
}

static t_term_err	pre_read(void)
{
	g_term.line = g_term.caps.hist.next;
	g_term.caps.index = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.caps.s_ios) == -1)
		return (TERM_ESETATTR);
	if (!(g_term.caps.mode & CAPS_MINS))
	{
		tputs(g_term.caps.modes.insert, 1, &putc_err);
		g_term.caps.mode |= CAPS_MINS;
	}
	return (TERM_EOK);
}

static t_term_err	post_read(t_term_err status)
{
	if (g_term.caps.mode & CAPS_MINS)
	{
		tputs(g_term.caps.modes.insert_end, 1,  &putc_err);
		g_term.caps.mode &= ~CAPS_MINS;
	}
	if (tcsetattr(STDIN_FILENO, TCSANOW, &g_term.caps.s_ios_orig) == -1
	&& status >= 0)
		status = TERM_ESETATTR;
	return (status);
}

/*
**	Read and parse an interactive terminal's input.
*/
t_term_err	term_read_caps(void)
{
	t_term_err	status;
	ssize_t		read_st;
	char		c;

	if ((status = pre_read()) != TERM_EOK)
		return (status); 
	while (status == TERM_EOK)
	{
		if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
			status = (read_st == 0) ? TERM_EEOF : TERM_EREAD;
		else if (c == ANSI_ESC)
			status = term_read_esc();
		else if (ft_iscntrl(c))
			status = term_cntrl(c);
		else if (ft_isascii(c))
			status = term_read_echo(c);
	}
	return (post_read(status));
}
