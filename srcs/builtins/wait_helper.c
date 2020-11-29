/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 02:07:01 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 06:35:33 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signals.h>
#include <job_control.h>

int				wait_process(t_process **target, int flags)
{
	(void)flags;
	if ((*target)->flags & EXITED)
		return (WEXITSTATUS((*target)->wstatus));
	if ((*target)->flags & STOPPED)
	{
		ft_dprintf(STDERR_FILENO, \
			"bash: warning: wait_for_job: job %lu[%d] is stopped\n", \
				background_index_get(g_session.nil, *target), (*target)->pid);
		return (148);
	}
	background_update(target);
	if ((*target)->flags & STOPPED)
	{
		ft_dprintf(STDERR_FILENO, \
			"bash: warning: wait_for_job: job %lu[%d] is stopped\n", \
				background_index_get(g_session.nil, *target), (*target)->pid);
		return (CMD_NOT_FOUND);
	}
	print_signal(2, *target, STANDART);
	return (WEXITSTATUS((*target)->wstatus));
}

bool			wait_delete()
{
	t_group		*next;

	if (!group_condition(g_session.groups, is_active))
	{
		zombies_list_remove_node(g_session.groups);
		history_session_remove_node(g_session.groups);
		(void)next;
		/*
		next = g_session.groups->next;
		g_session.groups->prev->next = next;
		next->prev = g_session.groups->prev;
		delete_group_input(&g_session.groups);
		delete_processes(g_session.groups);
		free(g_session.groups);
		g_session.groups = next;
		*/
		group_remove(&g_session.groups);
		return (true);
	}
	return (false);
}

static void		wait_group_loop(int *ret, int flags)
{
	while (g_session.groups->active_processes \
				!= g_session.groups->nil)
	{
		*ret = wait_process(&g_session.groups->active_processes, flags);
		g_session.groups->active_processes = \
			g_session.groups->active_processes->next;
	}
}

int				wait_group(t_process *leader, int flags, t_group *itself)
{
	t_group		*remember;
	t_process	*remember_leader;
	int ret;

	ret = 0;
	remember = g_session.groups;
	if (g_session.groups == itself)
		g_session.groups = g_session.groups->next;
	while (g_session.groups != g_session.nil)
	{
		if (g_session.groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session.groups->active_processes;
			wait_group_loop(&ret, flags);
			if (!wait_delete())
				g_session.groups->active_processes = remember_leader;
			break ;
		}
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = remember;
	return (ret);
}

int				wait_all_groups(int flags)
{
	t_group		*remember;
	t_group		*prev;
	int			ret;

	ret = 0;
	remember = g_session.groups;

	g_session.groups = g_session.nil->prev;
	while (g_session.groups != g_session.nil->next)
	{
		prev = g_session.groups->prev;
		if (g_session.groups->nil->next->flags & (SIGNALED | KILLED))
		{
			g_session.groups = prev;
			continue ;
		}
		ret = wait_group(g_session.groups->nil->next, flags, remember);
		g_session.groups = prev;
	}
	g_session.groups = remember;
	return (ret);
}