#include "term.h"

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
		if (str[len] == TERM_ESC && str[++len] == TERM_CSI)
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

/*
**	Append to an interactive terminal's input line.
*/
t_term_err	term_write(t_term *term, const char *input, size_t length)
{
	t_term_err status;

	status = TERM_EOK;
	if (length != 0)
	{
		if (write(STDERR_FILENO, input, length) == -1)
			status = TERM_EWRITE;
		else
		{
			status = line_insert(term->line, term->pos, input, length);
			term->pos += length;
		}
	}
	return (status);
}
