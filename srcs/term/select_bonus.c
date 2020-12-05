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

t_term_err	select_highlight(void)
{
	t_pos	pos;

	if (g_term.caps.selec.start != -1U && g_term.caps.selec.end != -1U)
	{
		/* ft_dprintf(2, "[SELECT] start: %lu, end: %lu\n",
			g_term.caps.selec.start, g_term.caps.selec.end); */
		pos = g_term.caps.cursor.real;
		caps_goto(&g_term.caps, &g_term.caps.cursor.origin);
		tputs(g_term.caps.ctrls.del_line, 1, &putc_err);
		if (write(STDERR_FILENO, g_term.line->data, g_term.caps.selec.start) == -1)
			return (TERM_EWRITE);
		tputs(g_term.caps.modes.standout, 1, &putc_err);
		if (write(STDERR_FILENO, g_term.line->data + g_term.caps.selec.start,
			g_term.caps.selec.end - g_term.caps.selec.start) == -1)
		{
			tputs(g_term.caps.modes.standout_end, 1, &putc_err);
			return (TERM_EWRITE);
		}
		tputs(g_term.caps.modes.standout_end, 1, &putc_err);
		if (write(STDERR_FILENO, g_term.line->data + g_term.caps.selec.end,
			g_term.line->len - g_term.caps.selec.end) == -1)
			return (TERM_EWRITE);
		caps_goto(&g_term.caps, &pos);
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
	t_pos	pos;

	if (g_term.caps.selec.start != -1U || g_term.caps.selec.end != -1U)
	{
		/* ft_dprintf(2, "[SELECT] clear\n"); */
		g_term.caps.selec.start = -1U;
		g_term.caps.selec.end = -1U;
		if (g_term.line)
		{
			pos = g_term.caps.cursor.real;
			caps_goto(&g_term.caps, &g_term.caps.cursor.origin);
			tputs(g_term.caps.ctrls.del_line, 1, &putc_err);
			if (write(STDERR_FILENO, g_term.line->data, g_term.line->len) == -1)
				return (TERM_EWRITE);
			caps_goto(&g_term.caps, &pos);
		}
	}
	return (TERM_EOK);
}
