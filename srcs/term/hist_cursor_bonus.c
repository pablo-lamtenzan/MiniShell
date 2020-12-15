/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hist_cursor_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by chamada           #+#    #+#             */
/*   Updated: 2020/12/12 15:58:34 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

t_term_err	term_set_line(t_line *line)
{
	t_term_err	status;

	status = TERM_EOK;
	term_clear_line();
	if (g_term.caps.hist.curr != g_term.caps.hist.next)
	{
		hist_commit(&g_term.caps.hist, g_term.line);
		free(g_term.line);
	}
	g_term.caps.hist.curr = line;
	if (!(g_term.line = line_dup(line)))
		status = TERM_EALLOC;
	else
		status = term_write(g_term.line->data, g_term.line->len);
	return (status);
}

t_term_err	term_prev_line(void)
{
	t_term_err	status;

	if (g_term.caps.hist.curr->prev)
		status = term_set_line(g_term.caps.hist.curr->prev);
	else
		status = TERM_EOK;
	return (status);
}

t_term_err	term_next_line(void)
{
	t_term_err	status;

	if (g_term.caps.hist.curr->next)
		status = term_set_line(g_term.caps.hist.curr->next);
	else
		status = TERM_EOK;
	return (status);
}
