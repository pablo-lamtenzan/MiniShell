/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 02:55:51 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:33:44 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

void			disown_process(t_process **target, int flags)
{
	if (flags & 1 && (*target)->flags & STOPPED)
	{
		(*target)->flags |= RESTRICT_OP;
		history_pop_front();
		return ;
	}
	if (flags & 4 && ((*target)->flags |= NO_HANGUP))
		return ;
	if ((*target)->flags & STOPPED)
		ft_dprintf(STDERR_FILENO, \
		"%s: warning: deleting stopped job %lu with process group %d\n", \
			g_session.name,
			background_index_get(g_session.nil, *target), \
			process_get_leader_pid(g_session.nil, *target));
	deadzombie_remove_node(*target);
	process_remove(target);
}

void			disown_group(t_process *leader, int flags, t_group *itself)
{
	t_group		*remember;

	remember = g_session.groups;
	if (g_session.groups == itself)
		g_session.groups = g_session.groups->next;
	while (g_session.groups != g_session.nil)
	{
		if (g_session.groups->nil->next->pid == leader->pid)
		{
			disown_core(flags);
			break ;
		}
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = remember;
}

void			disown_all_groups(int flags)
{
	t_group		*remember;
	t_group		*prev;

	remember = g_session.groups;
	g_session.groups = g_session.nil->prev;
	while (g_session.groups != g_session.nil->next)
	{
		prev = g_session.groups->prev;
		disown_group(g_session.groups->nil->next, flags, remember);
		g_session.groups = prev;
	}
	g_session.groups = remember;
}

int				disowm_delete(void)
{
	t_group		*fill;

	if (!group_condition(g_session.groups, is_active))
	{
		zombies_list_remove_node(g_session.groups);
		history_session_remove_node(g_session.groups);
		(void)fill;
		/*
		fill = g_session.groups;
		g_session.groups->prev->next = g_session.groups->next;
		g_session.groups->next->prev = g_session.groups->prev;
		delete_group_input(&fill);
		free(fill);
		fill = NULL;
		*/
		group_remove(&g_session.groups);
		return (true);
	}
	return (false);
}

void			disown_core(int flags)
{
	t_process	*next;
	t_process	*remember_leader;

	remember_leader = g_session.groups->active_processes;
	while (g_session.groups->active_processes != g_session.groups->nil)
	{
		next = g_session.groups->active_processes->next;
		disown_process(&g_session.groups->active_processes, flags);
		g_session.groups->active_processes = next;
	}
	if (!disowm_delete())
		g_session.groups->active_processes = remember_leader;
}
