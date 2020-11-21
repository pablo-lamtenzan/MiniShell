#include "term.h"

t_term_err	cursor_l(t_term *term)
{
	if (term->pos > 0)
	{
		term->pos--;
		tputs(term->caps.ctrl.left, 0, &ft_putchar);
	}
	return (TERM_EOK);
}

t_term_err	cursor_r(t_term *term)
{
	if (term->pos < term->line->len)
	{
		term->pos++;
		tputs(term->caps.ctrl.right, 0, &ft_putchar);
	}
	return (TERM_EOK);
}

t_term_err	cursor_start_line(t_term *term)
{
	tputs(tgoto(term->caps.ctrl.move_h, 0, term->origin), 0, &ft_putchar);
	term->pos = 0;
	return (TERM_EOK);
}

t_term_err	cursor_end_line(t_term *term)
{
	if (term->line && term->line->len)
	{
		term->pos = term->line->len;
		tputs(tgoto(term->caps.ctrl.move_h, 0, term->origin + term->pos),
			0, &ft_putchar);
	}
	return (TERM_EOK);
}
