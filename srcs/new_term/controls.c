#include "term.h"
/*
**	Clear the screen's content, keeping only the current line.
*/
t_term_err	term_clear_screen(t_term *term)
{
	if (term->caps.mode.clear)
	{
		tputs(term->caps.mode.clear, 0, &ft_putchar);
		if ((term->msg
		&& write(term->fds[2], term->msg, ft_strlen(term->msg)) == -1)
		|| write(1, term->line->data, term->line->len) == -1)
			return (TERM_EWRITE);
		term->pos = term->line->len;
	}
	return (TERM_EOK);
}

/*
**	Delete one character to the left of the cursor.
*/
t_term_err	term_backspace(t_term *term)
{
	if (term->pos > 0)
	{
		tputs(term->caps.ctrl.left, 0, &ft_putchar);
		tputs(term->caps.ctrl.del, 0, &ft_putchar);
		term->pos--;
		line_erase(term->line, term->pos, 1);
	}
	return (TERM_EOK);
}

/*
**	Clear the current line visually, without deleting it's content.
*/
t_term_err	term_clear_line(t_term *term)
{
	cursor_start_line(term);
	tputs(term->caps.ctrl.del_line, 0, &ft_putchar);
	return (TERM_EOK);
}

/*
**	Write the line ending character.
*/
t_term_err	term_new_line(t_term *term)
{
	if (write(term->fds[1], TERM_ENDL, sizeof(TERM_ENDL) - 1) == -1)
		return (TERM_EWRITE);
	if (term->line->len != 0)
	{
		ft_dprintf(2, "[PROMPT] result: '%s'\n", term->line->data);
		if ((!term->hist.next || term->line == term->hist.next)
		&& !(term->hist.next = line_new(10)))
			return (TERM_EALLOC);
		hist_add(&term->hist, term->line);
	}
	return (TERM_ENL);
}
