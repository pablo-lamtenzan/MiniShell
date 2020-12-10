/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:55:48 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:46:01 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>
#include <libft.h>
#include <sys/wait.h>

/*
** For each process in the target group returns true if the condition is true.
*/

bool			group_condition(t_group *target, bool (*condition)(t_process*))
{
	t_process	*process;

	process = target->nil->next;
	if (group_empty(target))
		return (false);
	while (process != target->nil)
	{
		if (condition(process))
			return (true);
		process = process->next;
	}
	return (false);
}

/*
** Return the group node in the job control data structure containing
** the target process given as argument.
*/

t_group			*group_get(t_process *target)
{
	t_group		*curr;
	t_group		*found;
	t_process	*leader;

	curr = g_session.groups;
	while (g_session.groups != g_session.nil)
	{
		leader = g_session.groups->active_processes;
		found = g_session.groups;
		while (g_session.groups->active_processes != g_session.groups->nil)
		{
			if (g_session.groups->active_processes->pid == target->pid)
			{
				g_session.groups->active_processes = leader;
				g_session.groups = curr;
				return (found);
			}
			g_session.groups->active_processes = \
				g_session.groups->active_processes->next;
		}
		g_session.groups->active_processes = leader;
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = curr;
	return (NULL);
}
