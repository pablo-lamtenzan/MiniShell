#include "term.h"

t_term_err	cursor_start_line(t_term *term)
{
	term->pos = 0;
	caps_goto(&term->caps, term->origin + term->pos);
	return (TERM_EOK);
}

t_term_err	cursor_end_line(t_term *term)
{
	if (term->line && term->line->len)
	{
		term->pos = term->line->len;
		caps_goto(&term->caps, term->origin + term->pos);
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
		caps_goto(&term->caps, term->origin + term->pos);
	}
	return (TERM_EOK);
}

t_term_err	cursor_prev_word(t_term *term)
{
	size_t	i;

	i = term->pos;
	while (i > 0 && ft_isspace(term->line->data[i - 1]))
		i--;
	while (i > 0 && !ft_isspace(term->line->data[i - 1]))
		i--;
	if (i != term->pos)
	{
		term->pos = i;
		caps_goto(&term->caps, term->origin + term->pos);
	}
	return (TERM_EOK);
}
