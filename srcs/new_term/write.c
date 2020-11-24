#include "term.h"

/*
**	Put a character to the standard-error output.
*/
int	putc_err(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/*
**	Overwrite an interactive terminal's prompt message.
*/
t_term_err			term_write_msg(t_term *t, const char *msg, size_t len)
{
	const bool		overflow = len > t->origin;
	const size_t	left = (overflow) ? len - t->origin : t->origin - len;
	const size_t	overlap = (overflow) ? t->origin : len;

	tputs(tgoto(t->caps.ctrl.move_h, 0, 0), 0, &putc_err);
	if (overlap != 0)
	{
		tputs(t->caps.mode.insert_end, 0, &putc_err);
		if (write(STDERR_FILENO, msg, overlap) == -1)
			return (TERM_EWRITE);
		tputs(t->caps.mode.insert, 0, &putc_err);
	}
	if (!overflow)
		tputs(t->caps.ctrl.del_n, left, &putc_err);
	else if (write(STDERR_FILENO, msg + t->origin, left) == -1)
		return (TERM_EWRITE);
	t->origin = len;
	tputs(tgoto(t->caps.ctrl.move_h, 0, t->pos), 0, &putc_err);
	return (TERM_EOK);
}

/*
**	Append to an interactive terminal's input line.
*/
t_term_err			term_write(t_term *term, const char *input, size_t length)
{
	t_term_err status;

	status = TERM_EOK;
	if (length != 0)
	{
		if (write(STDERR_FILENO, input, length) == -1)
			status = TERM_EWRITE;
		else
			status = line_insert(term->line, term->pos, input, length);
	}
	return (status);
}
