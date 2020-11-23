/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 19:28:58 by chamada           #+#    #+#             */
/*   Updated: 2020/11/12 05:22:52 by chamada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "term.h"

t_term_err	clip_copy(t_term *t)
{
	if (t->clip.select.start != -1U && t->clip.select.end != -1U)
	{
		free(t->clip.line.data);
		t->clip.line.data = NULL;
		t->clip.line.len = t->clip.select.end - t->clip.select.start;
		if (!t->clip.line.len || !t->line)
			return (TERM_EOK);
		if (!(t->clip.line.data = ft_substr(t->line->data,
		t->clip.select.start, t->clip.line.len)))
			return (TERM_EALLOC);
		t->pos = t->clip.select.start;
		t->clip.line.size = t->clip.line.len + 1;
		t->clip.line.next = NULL;
		t->clip.line.prev = NULL;
	}
	return (TERM_EOK);
}

t_term_err	clip_cut(t_term *t)
{
	t_term_err	status;

	if (t->clip.select.start != -1U && t->clip.select.end != -1U)
	{
		if ((status = clip_copy(t)) != TERM_EOK)
			return (status);
		tputs(tgoto(t->caps.ctrl.move_h, 0, t->origin + t->pos), 0, &putc_err);
		tputs(t->caps.ctrl.del_n, t->clip.line.len, &putc_err);
		// TODO: Fix line_erase with full line (Actually it seems to work...)
		line_erase(t->line, t->pos, t->clip.line.len);
		select_clear(t);
	}
	return (TERM_EOK);
}

t_term_err		clip_paste(t_term *t)
{
	if (!t->clip.line.len)
		return (TERM_EOK);
	select_clear(t);
	if (write(STDERR_FILENO, t->clip.line.data, t->clip.line.len) == -1)
		return (TERM_EWRITE);
	if (!line_insert(t->line, t->pos, t->clip.line.data, t->clip.line.len))
		return (TERM_EALLOC);
	t->pos += t->clip.line.len;
	return (TERM_EOK);
}

t_term_err	clip_clear(t_term *t)
{
	t->clip.line.len = 0;
	t->clip.line.size = 0;
	free(t->clip.line.data);
	t->clip.line.data = NULL;
	return (TERM_EOK);
}
