/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 11:58:30 by chamada           #+#    #+#             */
/*   Updated: 2020/12/12 23:08:53 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>
#include <job_control/session.h>
#include <job_control/background.h>

/*
**	Delete one character to the left of the cursor.
*/

t_term_err	term_backspace(void)
{
	if (g_term.caps.index > 0)
	{
		cursor_l();
		term_line_del(1);
	}
	return (TERM_EOK);
}

/*
**	Cancel the terminal's input line.
*/

t_term_err	term_interrupt(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (write(STDERR_FILENO, TERM_ENDL, sizeof(TERM_ENDL) - 1) == -1)
		return (TERM_EWRITE);
	if (!g_term.msg)
		g_term.caps.cursor.origin = g_term.caps.cursor.zero;
	else if ((status =
				term_origin(g_term.msg->data, g_term.msg->len)) != TERM_EOK)
		return (status);
	term_clear_eos();
	term_line_discard();
	return (status);
}

/*
**	End transmission when the line is empty and there are no jobs to be awaited.
*/

t_term_err	term_eof(void)
{
	t_term_err	status;

	status = TERM_EOK;
	if (g_term.line->len == 0)
	{
		if (g_session.exit_count == 1 || !is_background_stopped())
			status = TERM_EEOF;
		else
		{
			if ((g_session.exit_count = (g_session.exit_count == 0))
			&& (write(STDERR_FILENO,
				TERM_EXIT_JOBS, sizeof(TERM_EXIT_JOBS) - 1) == -1
			|| (g_term.msg && g_term.msg->len
			&& write(STDERR_FILENO, g_term.msg->data, g_term.msg->len) == -1)))
				status = TERM_EWRITE;
		}
	}
	else
		term_line_del(1);
	return (status);
}
