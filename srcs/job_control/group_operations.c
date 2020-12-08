/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:55:48 by pablo             #+#    #+#             */
/*   Updated: 2020/12/08 21:13:02 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <sys/wait.h>

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
