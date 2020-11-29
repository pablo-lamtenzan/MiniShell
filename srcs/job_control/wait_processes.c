/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:56:03 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 03:07:02 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>
#include <signals.h>

/*
replaces wait_processes_v2
*/
t_exec_status	wait_processes(t_exec_status st)
{
	t_group*	group;
	t_process*	leader;

	g_session.flags |= RESTRICT_CATCH;
	if (!(group = g_session.groups) || g_session.groups->active_processes == g_session.groups->nil)
	{
		// TO DO: Check that condition with more group members
		if (!(g_session.groups->active_processes->flags & SIGNALED))
			group_pop_front();
		deadzombies_print();
		zombies_list_purge_exited_groups();
		g_session.flags &= ~RESTRICT_CATCH;
		return (st);
	}
	leader = group->active_processes;
	while (group->active_processes != group->nil)
	{
		g_session.flags &= ~OPEN_PRINT;
		background_update(&group->active_processes);
		if (WIFSIGNALED(group->active_processes->wstatus) \
				|| WIFSTOPPED(group->active_processes->wstatus))
			print_signal(STDERR_FILENO, group->active_processes, STANDART);
		group->active_processes = group->active_processes->next;
	}
	group->active_processes = leader;
	group_return_handler();
	deadzombies_print();
	if (!group_condition(g_session.groups, is_active))
		group_pop_front();
	zombies_list_purge_exited_groups();
	g_session.flags &= ~RESTRICT_CATCH;
	return (st);
}