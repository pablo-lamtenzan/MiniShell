/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 10:51:59 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/09 10:51:59 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

/*
**	Clear the screen's content, keeping only the current line.
*/
t_term_err	term_clear_screen(void)
{
	const t_pos	pos = g_term.caps.cursor.pos;
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.caps.ctrls.clear)
	{
		tputs(g_term.caps.ctrls.clear, 1, &putc_err);
		if ((!g_term.msg ||
		(status = term_origin(g_term.msg->data, g_term.msg->len)) == TERM_EOK))
			status = term_write(g_term.line->data, g_term.line->len);
		caps_goto(&g_term.caps, pos);
	}
	return (status);
}

// TODO: Check if delete-mode needs to be activated before clearing parts

/*
**	Clear the screen from the cursor to the end of the screen.
*/
t_term_err	term_clear_eos(void)
{
	const int	x = g_term.caps.cursor.pos.x;

	if (x)
	{
		tputs(g_term.caps.ctrls.del_eol, 1, &putc_err);
		if (x + g_term.line->len - g_term.caps.index >= (size_t)g_term.caps.width)
		{
			caps_goto_x(&g_term.caps, 0);
			caps_goto_y(&g_term.caps, g_term.caps.cursor.pos.y + 1);
			tputs(g_term.caps.ctrls.del_eos, 1, &putc_err);
			caps_goto_y(&g_term.caps, g_term.caps.cursor.pos.y - 1);
			caps_goto_x(&g_term.caps, x);
		}
	}
	else
		tputs(g_term.caps.ctrls.del_eos, 1, &putc_err);
	return (TERM_EOK);
}

/*
**	Clear the current line visually, without deleting it's content.
*/
t_term_err	term_clear_line(void)
{
	caps_goto(&g_term.caps, g_term.caps.cursor.origin);
	g_term.caps.index = 0;
	term_clear_eos();
	return (TERM_EOK);
}
