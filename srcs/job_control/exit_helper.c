/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:30:21 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:16:25 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>
#include <job_control/background.h>
#include <libft.h>
#include <term/term.h>


#include <stdlib.h>

bool		is_stopped(t_process *target)
{
	return (target->flags & STOPPED);
}

/*
** Check if the parent (our shell) is allowed to exit.
*/

void		handle_exit_with_active_background(int exit_status,
		t_session *session)
{
	if (session->flags & PIPED_CMD)
		return ;
	if (session->exit_count++ == 2 || !is_background_stopped())
	{
		term_destroy();
		session_end(&g_session);
		exit(exit_status);
	}
	else
		write(STDERR_FILENO, "There are stopped jobs.\n", 25);
}

/*
** Uptate the stopped jobs exit count.
*/

void		update_exit_count(const char *name)
{
	if (!ft_strncmp(name, "exit", 5) && g_session.exit_count == 1)
		g_session.exit_count++;
	else
		g_session.exit_count = 0;
}
