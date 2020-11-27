/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:56:03 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 03:43:45 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

/*
replaces wait_processes_v2
*/
t_exec_status	wait_processes(t_exec_status st)
{
	t_group*	group;
	t_process*	leader;

	if (!(group = g_session->groups) || g_session->groups->active_processes == g_session->groups->nil)
	{
		// TO DO: Check that condition with more group members
		if (!(g_session->groups->active_processes->flags & SIGNALED))
			group_pop_front();
		print_endzombies();
		remove_exited_zombies();
		return (st);
	}
	leader = group->active_processes;
	while (group->active_processes != group->nil)
	{
		g_session->open_print = false;
		update_background(&group->active_processes, true);
		if (WIFSIGNALED(group->active_processes->wstatus) \
				|| WIFSTOPPED(group->active_processes->wstatus))
				// TO DO: mode is define
			print_signal(STDERR_FILENO, group->active_processes, 0);
		group->active_processes = group->active_processes->next;
	}
	group->active_processes = leader;
	get_group_return();
	print_endzombies();
	if (!group_condition(g_session->groups, is_active))
		group_pop_front();
	remove_exited_zombies();
	return (st);
}