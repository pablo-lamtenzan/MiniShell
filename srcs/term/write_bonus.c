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

// FIXME
/*
**	Append to an interactive terminal's input line.
*/
t_term_err	term_write(const char *input, size_t length)
{
	t_term_err	status;
	const t_pos	wrapped = (t_pos){
		length % (g_term.caps.width - 1),
		length / (g_term.caps.width - 1),
	};
	const int	space_left = g_term.caps.width - g_term.caps.cursor.real.x;
	const int	first_len = ((size_t)space_left < length) ? space_left : length;
	int			y;

	status = TERM_EOK;
	if ((y = (wrapped.y && first_len != 0))
	&& (write(STDERR_FILENO, input, first_len) == -1
	|| tputs(tgetstr("nw", NULL), 1, putc_err) == -1))
		return (TERM_EWRITE);
	while (y < wrapped.y
	&& tputs(tgetstr("nw", NULL), 1, putc_err) != -1
	&& write(STDERR_FILENO, input, g_term.caps.width) != -1)
	{
		y++;
		input += g_term.caps.width;
	}
	if (y < wrapped.y
	|| (wrapped.x && write(STDERR_FILENO, input, wrapped.x) == -1))
		status = TERM_EWRITE;
	g_term.caps.index += length;
	g_term.caps.cursor.real.x += wrapped.x;
	g_term.caps.cursor.real.y += wrapped.y;
	g_term.caps.cursor.pos.x += wrapped.x;
	g_term.caps.cursor.pos.y += wrapped.y;
	return (status);
}

t_term_err	old_term_write(const char *input, size_t length)
{
	t_term_err	status;
	t_pos		wrapped;
	int			empty;
	int			y;
	size_t		new_index;

	status = TERM_EOK;
	if (length != 0)
	{
		new_index = g_term.caps.index + length;
		g_term.caps.index = new_index;
		if ((size_t)(empty = g_term.caps.width - g_term.caps.cursor.real.x) < length)
		{
			if (write(STDERR_FILENO, input, empty) == -1)
				return (TERM_EWRITE);
			input += empty;
			new_index -= empty;
			wrapped.x = new_index % g_term.caps.width;
			wrapped.y = new_index / g_term.caps.width;
			y = 0;
			while (y < wrapped.y && write(STDERR_FILENO,
				input + y * g_term.caps.width, g_term.caps.width) != -1)
			{
				y++;
				ft_dprintf(2, "WRAP");
				tputs(g_term.caps.ctrls.scroll_down, 1, &putc_err);
			}
			ft_dprintf(2, "y: %d, wrapped.y: %d\n", y, wrapped.y);
			if (y != wrapped.y || write(STDERR_FILENO,
				input + y * g_term.caps.width, wrapped.x) == -1)
				return (TERM_EWRITE);
			g_term.caps.cursor.pos.x = (g_term.caps.cursor.pos.x + wrapped.x) % g_term.caps.width;
			g_term.caps.cursor.pos.y += wrapped.y;
			g_term.caps.cursor.real.x += wrapped.x;
			g_term.caps.cursor.real.y += wrapped.y;
		}
		else
		{
			if (write(STDERR_FILENO, input, length) == -1)
				return (TERM_EWRITE);
			g_term.caps.cursor.pos.x += length;
			g_term.caps.cursor.real.x += length;
		}
		//ft_dprintf(2, "EMPTY: %d", empty);
	}
	return (status);
}

// TODO: Set msg line here
t_term_err	term_origin(const char *input, size_t length)
{
	t_term_err		status;
	const size_t	graphical_len = strglen(input);

	//caps_goto(&g_term.caps, &g_term.caps.cursor.zero);
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
