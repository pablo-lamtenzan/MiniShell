#include <term/term.h>

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

t_term_err	cursor_write(const char *input, size_t length)
{
	if (write(STDERR_FILENO, input, length) == -1)
		return (TERM_EWRITE);
	g_term.caps.index += length;
	g_term.caps.cursor.real = (t_pos) {
		(g_term.caps.cursor.origin.x + g_term.caps.index)
			% (g_term.caps.width),
		(g_term.caps.cursor.origin.x + g_term.caps.index)
			/ (g_term.caps.width) + g_term.caps.cursor.origin.y,
	};
	if (g_term.caps.cursor.real.x == 0)
		cursor_d();
	return (TERM_EOK);
}

t_term_err	term_write(const char *input, size_t length)
{
	size_t		remaining;
	t_pos		pos;
	size_t		index;
	t_term_err	status;

	status = TERM_EOK;
	if (length)
	{
		// TODO: Get error return
		cursor_write(input, length);
		if ((remaining = g_term.line->len - g_term.caps.index)
		&& (g_term.caps.cursor.real.x == 0
		|| g_term.caps.cursor.real.x + remaining >= (size_t)g_term.caps.width))
		{
			pos = g_term.caps.cursor.real;
			index = g_term.line->len - remaining;
			tputs(g_term.caps.modes.insert_end, 1, &putc_err);
			status = cursor_write(g_term.line->data + index, remaining);
			tputs(g_term.caps.modes.insert, 1, &putc_err);
			caps_goto(&g_term.caps, &pos);
			g_term.caps.index -= remaining;
		}
	}
	return (status);
}

// TODO: Set msg line here
t_term_err	term_origin(const char *input, size_t length)
{
	t_term_err		status;
	const size_t	graphical_len = strglen(input);

	status = TERM_EOK;
	if (length && write(STDERR_FILENO, input, length) == -1)
		status = TERM_EWRITE;
	else if (g_term.has_caps)
	{
		g_term.caps.cursor.origin = (t_pos) {
			graphical_len % g_term.caps.width,
			graphical_len / g_term.caps.width
		};
		g_term.caps.cursor.real = g_term.caps.cursor.origin;
		if (g_term.caps.cursor.real.x == 0)
			cursor_d();
	}
	return (status);
}
