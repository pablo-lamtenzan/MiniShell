/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_terminated.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 04:43:25 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 04:45:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>

static void				next_is_killed(bool *stop)
{
	t_group*			fill;

	if (g_session.groups->next->active_processes->flags & SIGNALED \
			&& !(g_session.groups->next->active_processes->flags & KILLED))
	{
		g_session.groups->next->active_processes->flags &= ~SIGNALED;
		fill = g_session.groups->next;
		g_session.groups->next->next->prev = g_session.groups;
		g_session.groups->next = g_session.groups->next->next;
		free(fill);
		*stop = true;
	}
}

void					keep_alive_killed_processes()
{
	bool				stop;

	stop = false;
	if (!session_empty() && !group_empty(g_session.groups) \
			&& g_session.groups->active_processes)
	{
		if (g_session.groups->active_processes->flags & SIGNALED \
				&& !(g_session.groups->active_processes->flags & KILLED))
		{
			g_session.groups->active_processes->flags &= ~SIGNALED;
			stop = true;
			group_pop_front();
		}
		if (g_session.groups->next && g_session.groups->next != g_session.nil)
			next_is_killed(&stop);
		if (!stop && g_session.groups->active_processes->flags & KILLED)
			g_session.groups->active_processes->flags &= ~KILLED;
	} 
}