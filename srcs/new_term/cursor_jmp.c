#include "term.h"

t_term_err	cursor_start_line(t_term *term)
{
	tputs(tgoto(term->caps.ctrl.move_h, 0, term->origin), 0, &putc_err);
	term->pos = 0;
	return (TERM_EOK);
}

t_term_err	cursor_end_line(t_term *term)
{
	if (term->line && term->line->len)
	{
		term->pos = term->line->len;
		tputs(tgoto(term->caps.ctrl.move_h, 0, term->origin + term->pos),
			0, &putc_err);
	}
	return (TERM_EOK);
}

t_term_err	cursor_next_word(t_term *term)
{
	size_t	i;

	i = term->pos;
	while (i < term->line->len && ft_isspace(term->line->data[i]))
		i++;
	while (i < term->line->len && !ft_isspace(term->line->data[i]))
		i++;
	if (i != term->pos)
	{
		term->pos = i;
		tputs(term->caps.ctrl.move_h, term->origin + term->pos, &putc_err);
	}
	return (TERM_EOK);
}

t_term_err	cursor_prev_word(t_term *term)
{
	size_t	i;

	i = term->pos;
	while (i > 0 && ft_isspace(term->line->data[i]))
		i--;
	while (i > 0 && !ft_isspace(term->line->data[i]))
		i--;
	if (i != term->pos)
	{
		term->pos = i;
		tputs(term->caps.ctrl.move_h, term->origin + term->pos, &putc_err);
	}
	return (TERM_EOK);
}
