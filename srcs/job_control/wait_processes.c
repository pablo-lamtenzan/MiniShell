/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:56:03 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:48:03 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control/background.h>
#include <job_control/conditions.h>
#include <cross_plateform_signals.h>
#include <signal_handler.h>
#include <signals_print.h>

static void		end_parent_execs(void)
{
	if (!(group_condition(g_session.groups, is_signaled)))
		group_pop_front();
	deadzombies_print();
	zombies_list_purge_exited_zombies();
	zombies_list_purge_exited_groups();
	signal(SIGCHLD, zombies_catcher);
}

static void		end_child_execs(void)
{
	if (!(g_session.flags & BUILTIN))
		group_return_handler();
	deadzombies_print();
	if (!group_condition(g_session.groups, is_active))
		group_pop_front();
	zombies_list_purge_exited_zombies();
	zombies_list_purge_exited_groups();
	signal(SIGCHLD, zombies_catcher);
}

/*
** Wait all the (asyncronous) childs, update and print their data
** in the job control data structure, destroy dinamically the needed
** ressources, print if necesary the exited zombie procesess data and
** if necesary too, correct the execution return status.
*/

t_exec_status	wait_processes(t_exec_status st)
{
	t_group		*group;
	t_process	*leader;

	signal(SIGCHLD, SIG_DFL);
	if (!(group = g_session.groups) \
		|| g_session.groups->active_processes == g_session.groups->nil)
	{
		end_parent_execs();
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
	end_child_execs();
	return (st);
}
