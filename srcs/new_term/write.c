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
	//				  insert 3         delete 5
	// 8        3     3     5          8        3      3   3
	// 01234567 012   012   012345     01234567 012    012 012
	// minish>  xxx | def : minish> -> defish>  xxx -> def xxx

	//ft_dprintf(2, "len: %lu, origin: %lu\n", len, t->origin);
	const bool		overflow = len > t->origin;
	const size_t	overlap = (overflow) ? t->origin : len;
	const size_t	remainder = (overflow) ? len - t->origin : t->origin - len;

	//ft_dprintf(2, "overlap: %lu, remainder: %lu\n", overlap, remainder);
	tputs(tgoto(t->caps.ctrl.move_h, 0, 0), 0, &putc_err);
	if (overlap != 0)
	{
		tputs(t->caps.mode.insert_end, 0, &putc_err);
		if (write(STDERR_FILENO, msg, overlap) == -1)
			return (TERM_EWRITE);
		tputs(t->caps.mode.insert, 0, &putc_err);
	}
	if (remainder != 0)
	{
		if (!overflow)
		{
			//tputs(tgoto(t->caps.ctrl.move_h, 0, len - 1), 0, &putc_err);
			tputs(t->caps.ctrl.del_n, remainder, &putc_err);
		}
		else if (write(STDERR_FILENO, msg + t->origin, remainder) == -1)
			return (TERM_EWRITE);
	}
	t->origin = len;
	tputs(tgoto(t->caps.ctrl.move_h, 0, t->origin + t->pos), 0, &putc_err);
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
