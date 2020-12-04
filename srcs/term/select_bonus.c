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
	if (g_term.selec.start != -1U && g_term.selec.end != -1U)
	{
		/* ft_dprintf(2, "[SELECT] start: %lu, end: %lu\n",
			g_term.selec.start, g_term.selec.end); */
		caps_goto(&g_term.caps, g_term.origin);
		tputs(g_term.caps.ctrl.del_line, 1, &putc_err);
		if (write(STDERR_FILENO, g_term.line->data, g_term.selec.start) == -1)
			return (TERM_EWRITE);
		tputs(g_term.caps.mode.standout, 1, &putc_err);
		if (write(STDERR_FILENO, g_term.line->data + g_term.selec.start,
			g_term.selec.end - g_term.selec.start) == -1)
			return (TERM_EWRITE);
		tputs(g_term.caps.mode.standout_end, 1, &putc_err);
		if (write(STDERR_FILENO, g_term.line->data + g_term.selec.end,
			g_term.line->len - g_term.selec.end) == -1)
			return (TERM_EWRITE);
		caps_goto(&g_term.caps, g_term.origin + g_term.pos);
	}
	return (TERM_EOK);
}

t_term_err	select_left(void)
{
	if (g_term.pos > 0)
	{
		if (g_term.selec.start == -1U || g_term.selec.end == -1U)
		{
			g_term.selec.start = g_term.pos;
			g_term.selec.end = g_term.pos;
		}
		else
		{
			cursor_l();
			if (g_term.selec.start == g_term.pos + 1)
				g_term.selec.start = g_term.pos;
			else
				g_term.selec.end = g_term.pos;
			select_highlight();
		}
	}
	return (TERM_EOK);
}

t_term_err	select_right(void)
{
	if (g_term.pos < g_term.line->len)
	{
		if (g_term.selec.start == -1U || g_term.selec.end == -1U)
		{
			g_term.selec.start = g_term.pos;
			g_term.selec.end = g_term.pos;
		}
		else
		{
			cursor_r();
			if (g_term.selec.end == g_term.pos - 1)
				g_term.selec.end = g_term.pos;
			else
				g_term.selec.start = g_term.pos;
			select_highlight();
		}
	}
	return (TERM_EOK);
}

t_term_err	select_clear(void)
{
	if (g_term.selec.start != -1U || g_term.selec.end != -1U)
	{
		/* ft_dprintf(2, "[SELECT] clear\n"); */
		g_term.selec.start = -1U;
		g_term.selec.end = -1U;
		if (g_term.line)
		{
			caps_goto(&g_term.caps, g_term.origin);
			tputs(g_term.caps.ctrl.del_line, 1, &putc_err);
			if (write(STDERR_FILENO, g_term.line->data, g_term.line->len) == -1)
				return (TERM_EWRITE);
			caps_goto(&g_term.caps, g_term.origin + g_term.pos);
		}
	}
	return (TERM_EOK);
}
