/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:55:48 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 07:55:12 by pablo            ###   ########.fr       */
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

void			group_return_handler(void)
{
	t_process	*leader;

	if (group_condition(g_session.groups, is_active))
	{
		leader = g_session.groups->active_processes;
		while (g_session.groups->active_processes != g_session.groups->nil)
		{
			if (g_session.groups->active_processes->flags \
						& (STOPPED | BACKGROUND))
			{
				if (g_session.groups->active_processes->prev \
						== g_session.groups->nil)
					g_session.st = SIGNAL_BASE + \
						WSTOPSIG(g_session.groups->active_processes->wstatus);
				else
					g_session.st = SIGNAL_BASE + \
						g_session.groups->active_processes->prev->ret;
				break ;
			}
			g_session.groups->active_processes = \
				g_session.groups->active_processes->next;
		}
		g_session.groups->active_processes = leader;
	}
}
