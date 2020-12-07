#include <term/term.h>

/*
**	Put a character to the standard-error output.
*/
int			putc_err(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/*
**	Get a c-string's graphical character count
*/
size_t		strglen(const char *str)
{
	size_t	len;
	size_t	glen;

	len = 0;
	glen = 0;
	while (str[len])
	{
		if (str[len] == ANSI_ESC && str[++len] == ANSI_CSI)
		{
			while ((str[++len] & 0xF0) == 0x30) // param
				;
			while ((str[len] & 0xF0) == 0x20) // inter
				len++;
			if ((str[len] & 0xC0) == 0x40) // final
				len++;
		}
		else if (ft_isprint(str[len++])) // graphical
			glen++;
	}
	return (glen);
}

// TODO: Handle or reject ic
// TODO: Handle or reject 1-width terminal

t_term_err	term_write(const char *input, size_t length)
{
	t_term_err	status;

	status = TERM_EOK;
	if (length)
	{
		// TODO: tputs(tparm(tgetstr("IC", NULL), length), 1, &putc_err);	
		if (write(1, input, length) == -1)
			status = TERM_EWRITE;
		g_term.caps.index += length;
		g_term.caps.cursor.real = (t_pos) {
			(g_term.caps.cursor.origin.x + g_term.caps.index)
				% (g_term.caps.width),
			(g_term.caps.cursor.origin.x + g_term.caps.index)
				/ (g_term.caps.width) + g_term.caps.cursor.origin.y,
		};
		if (g_term.caps.cursor.real.x == 0)
		{
			if (g_term.caps.index < g_term.line->len)
				tputs(g_term.caps.ctrls.down, 1, &putc_err);
			else
				tputs(g_term.caps.ctrls.scroll_down, 1, &putc_err);
		}
	}
	return (status);
}

// TODO: Set msg line here
t_term_err	term_origin(const char *input, size_t length)
{
	t_term_err		status;
	const size_t	graphical_len = strglen(input);

	if (length && write(STDERR_FILENO, input, length) == -1)
		status = TERM_EWRITE;
	else
	{
		status = TERM_EOK;
		g_term.caps.cursor.origin = (t_pos) {
			graphical_len % g_term.caps.width,
			graphical_len / g_term.caps.width
		};
		g_term.caps.cursor.real = g_term.caps.cursor.origin;
	}
	return (status);
}
