#include "term.h"

t_term_err	cursor_l(t_term *term)
{
	if (term->pos > 0)
	{
		term->pos--;
		tputs(term->caps.ctrl.left, 0, &putc_err);
	}
	return (TERM_EOK);
}

t_term_err	cursor_r(t_term *term)
{
	if (term->pos < term->line->len)
	{
		term->pos++;
		tputs(term->caps.ctrl.right, 0, &putc_err);
	}
	return (TERM_EOK);
}
