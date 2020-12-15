/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 18:57:39 by chamada           #+#    #+#             */
/*   Updated: 2020/11/28 01:55:54 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

t_term_err	cursor_write_so(const char *data, size_t length)
{
	t_term_err	status;

	status = TERM_EOK;
	tputs(g_term.caps.modes.standout, 1, &putc_err);
	g_term.caps.mode |= CAPS_MSO;
	if ((status = cursor_write(data, length)) != TERM_EOK)
	{
		tputs(g_term.caps.modes.standout_end, 1, &putc_err);
		g_term.caps.mode &= ~CAPS_MSO;
		return (status);
	}
	tputs(g_term.caps.modes.standout_end, 1, &putc_err);
	g_term.caps.mode &= ~CAPS_MSO;
	return (status);
}

t_term_err	select_highlight(void)
{
	int				index;
	t_pos			pos;
	t_term_err		status;
	const t_select	*selec = &g_term.caps.selec;

	if (selec->start != -1U && selec->end != -1U)
	{
		pos = g_term.caps.cursor.pos;
		index = g_term.caps.index;
		term_clear_line();
		if ((status = cursor_write(g_term.line->data, selec->start)) != TERM_EOK
		|| (status = cursor_write_so(g_term.line->data + selec->start,
			selec->end - selec->start)) != TERM_EOK
		|| (status = cursor_write(g_term.line->data + selec->end,
			g_term.line->len - selec->end)) != TERM_EOK)
			return (status);
		caps_goto(&g_term.caps, pos);
		g_term.caps.index = index;
	}
	return (TERM_EOK);
}

t_term_err	select_left(void)
{
	if (g_term.caps.index > 0)
	{
		if (g_term.caps.selec.start == -1U || g_term.caps.selec.end == -1U)
		{
			g_term.caps.selec.start = g_term.caps.index;
			g_term.caps.selec.end = g_term.caps.index;
		}
		else
		{
			cursor_l();
			if (g_term.caps.selec.start == g_term.caps.index + 1)
				g_term.caps.selec.start = g_term.caps.index;
			else
				g_term.caps.selec.end = g_term.caps.index;
			select_highlight();
		}
	}
	return (TERM_EOK);
}

t_term_err	select_right(void)
{
	if (g_term.caps.index < g_term.line->len)
	{
		if (g_term.caps.selec.start == -1U || g_term.caps.selec.end == -1U)
		{
			g_term.caps.selec.start = g_term.caps.index;
			g_term.caps.selec.end = g_term.caps.index;
		}
		else
		{
			cursor_r();
			if (g_term.caps.selec.end == g_term.caps.index - 1)
				g_term.caps.selec.end = g_term.caps.index;
			else
				g_term.caps.selec.start = g_term.caps.index;
			select_highlight();
		}
	}
	return (TERM_EOK);
}

t_term_err	select_clear(void)
{
	t_term_err	status;
	t_pos		pos;
	int			index;

	status = TERM_EOK;
	if (g_term.caps.selec.start != -1U || g_term.caps.selec.end != -1U)
	{
		g_term.caps.selec = (t_select){-1U, -1U};
		if (g_term.line)
		{
			pos = g_term.caps.cursor.pos;
			index = g_term.caps.index;
			term_clear_line();
			status = term_write(g_term.line->data, g_term.line->len);
			caps_goto(&g_term.caps, pos);
			g_term.caps.index = index;
		}
	}
	return (status);
}
