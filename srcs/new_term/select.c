/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 18:57:39 by chamada           #+#    #+#             */
/*   Updated: 2020/11/25 17:40:57 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "term.h"

t_term_err	select_highlight(t_term *t)
{
	if (t->clip.select.start != -1U && t->clip.select.end != -1U)
	{
		/* ft_dprintf(2, "[SELECT] start: %lu, end: %lu\n",
			t->clip.select.start, t->clip.select.end); */
		caps_goto(&t->caps, t->origin);
		tputs(t->caps.ctrl.del_line, 1, &putc_err);
		if (write(STDERR_FILENO, t->line->data, t->clip.select.start) == -1)
			return (TERM_EWRITE);
		tputs(t->caps.mode.standout, 1, &putc_err);
		if (write(STDERR_FILENO, t->line->data + t->clip.select.start,
			t->clip.select.end - t->clip.select.start) == -1)
			return (TERM_EWRITE);
		tputs(t->caps.mode.standout_end, 1, &putc_err);
		if (write(STDERR_FILENO, t->line->data + t->clip.select.end,
			t->line->len - t->clip.select.end) == -1)
			return (TERM_EWRITE);
		caps_goto(&t->caps, t->origin + t->pos);
	}
	return (TERM_EOK);
}

t_term_err	select_left(t_term *t)
{
	if (t->pos > 0)
	{
		if (t->clip.select.start == -1U || t->clip.select.end == -1U)
		{
			t->clip.select.start = t->pos;
			t->clip.select.end = t->pos;
		}
		else
		{
			cursor_l(t);
			if (t->clip.select.start == t->pos + 1)
				t->clip.select.start = t->pos;
			else
				t->clip.select.end = t->pos;
			select_highlight(t);
		}
	}
	return (TERM_EOK);
}

t_term_err	select_right(t_term *t)
{
	if (t->pos < t->line->len)
	{
		if (t->clip.select.start == -1U || t->clip.select.end == -1U)
		{
			t->clip.select.start = t->pos;
			t->clip.select.end = t->pos;
		}
		else
		{
			cursor_r(t);
			if (t->clip.select.end == t->pos - 1)
				t->clip.select.end = t->pos;
			else
				t->clip.select.start = t->pos;
			select_highlight(t);
		}
	}
	return (TERM_EOK);
}

t_term_err	select_clear(t_term *t)
{
	if (t->clip.select.start != -1U || t->clip.select.end != -1U)
	{
		/* ft_dprintf(2, "[SELECT] clear\n"); */
		t->clip.select.start = -1U;
		t->clip.select.end = -1U;
		if (t->line)
		{
			caps_goto(&t->caps, t->origin);
			tputs(t->caps.ctrl.del_line, 1, &putc_err);
			if (write(STDERR_FILENO, t->line->data, t->line->len) == -1)
				return (TERM_EWRITE);
			caps_goto(&t->caps, t->origin + t->pos);
		}
	}
	return (TERM_EOK);
}
