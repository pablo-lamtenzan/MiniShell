/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_line_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 11:58:30 by chamada           #+#    #+#             */
/*   Updated: 2020/12/13 00:35:21 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

/*
**	Write the line ending character and add the current line to history.
*/

t_term_err	term_line_new(void)
{
	if (g_term.line->len)
	{
		term_clear_line();
		if (write(STDERR_FILENO, g_term.line->data, g_term.line->len) == -1)
			return (TERM_EWRITE);
		else if ((!g_term.caps.hist.next
		|| g_term.line == g_term.caps.hist.next)
		&& !(g_term.caps.hist.next = line_new(10)))
			return (TERM_EALLOC);
		else
			hist_add(&g_term.caps.hist, g_term.line);
	}
	else if (g_term.caps.hist.curr != g_term.caps.hist.next)
	{
		line_clear(&g_term.line);
		g_term.caps.hist.curr = g_term.caps.hist.next;
		g_term.line = g_term.caps.hist.curr;
	}
	if (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) - 1) == -1)
		return (TERM_EWRITE);
	return (TERM_ENL);
}

/*
**	Delete n characters from the terminal's input line, starting at the current
**	cursor position.
*/

t_term_err	term_line_del(size_t n)
{
	const t_pos		pos = g_term.caps.cursor.pos;
	const size_t	index = g_term.caps.index;
	const size_t	remaining = g_term.line->len - n - index;
	t_term_err		status;
	bool			toggle_ins;

	status = TERM_EOK;
	if (pos.x + n < (size_t)g_term.caps.width
	&& pos.x + remaining < (size_t)g_term.caps.width)
		caps_delete(&g_term.caps, n);
	else
	{
		if ((toggle_ins = (g_term.caps.mode & CAPS_MINS)))
			tputs(g_term.caps.modes.insert_end, 1, &putc_err);
		status = cursor_write(g_term.line->data + index, remaining);
		if (toggle_ins)
			tputs(g_term.caps.modes.insert, 1, &putc_err);
		term_clear_eos();
		caps_goto(&g_term.caps, pos);
		g_term.caps.index = index;
	}
	line_erase(g_term.line, index, n);
	return (status);
}

/*
**	Discard a line's content.
*/

t_term_err	term_line_discard(void)
{
	g_term.caps.selec = (t_select){-1U, -1U};
	if (g_term.line != g_term.caps.hist.next)
	{
		line_clear(&g_term.line);
		g_term.caps.hist.curr = g_term.caps.hist.next;
		g_term.line = g_term.caps.hist.next;
	}
	*g_term.line->data = '\0';
	g_term.line->len = 0;
	g_term.caps.index = 0;
	return (TERM_EOK);
}

/*
**	Kill a line's content on the left of the cursor.
*/

t_term_err	term_line_kill(void)
{
	const size_t	index = g_term.caps.index;

	caps_goto(&g_term.caps, g_term.caps.cursor.origin);
	g_term.caps.index = 0;
	term_line_del(index);
	return (TERM_EOK);
}
