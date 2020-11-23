/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:39:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 15:59:36 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

// for the momment keep them

t_group*	get_group(t_process* target)
{
	t_group*	remember;
	t_process*	remember_leader;
	t_group*	found;

	remember = g_session->groups;
	while (g_session->groups != g_session->nil)
	{
		remember_leader = g_session->groups->active_processes;
		found = g_session->groups;
		while (g_session->groups->active_processes != g_session->groups->nil)
		{
			if (g_session->groups->active_processes->pid == target->pid)
			{
				g_session->groups->active_processes = remember_leader;
				g_session->groups = remember;
				if (PRINT_DEBUG)
					ft_dprintf(2, "[GET GROUP FOUND: %p]\n", found);
				return (found);
			}
			g_session->groups->active_processes = g_session->groups->active_processes->next;
		}
		g_session->groups->active_processes = remember_leader;
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
	return (NULL);
}

void		update_background(t_process **target, bool wait)
{
	//ft_dprintf(2, "[UPDATE V2--1]ACTIVE PROCESSES: %p\n", (*target));
	if (wait) // need for kill
	{
		(*target)->flags &= ~BACKGROUND;
		while (waitpid((*target)->pid, &(*target)->wstatus, WUNTRACED) <= 0)
			;
		if (PRINT_DEBUG)
			ft_dprintf(2, "[WAIING...][pid=\'%d\'][wstatus=\'%d\']\n", (*target)->pid, (*target)->wstatus);
	}
	// exited and not stopped
	// HERE CAN REMOVE FLAGS IN ALL CASES AND ADD IT IF NECESARRY
	if (WIFEXITED((*target)->wstatus) || !WIFSTOPPED((*target)->wstatus))
	{
		if (PRINT_DEBUG)
			ft_dprintf(2, "[PROCESS EXITS]\n");
		(*target)->flags &= ~STOPPED;
		(*target)->flags |= EXITED;
	}
	// stopped
	else
	{
		if (PRINT_DEBUG)
			ft_dprintf(2, "[PROCESS DOESN'T EXIT]\n");
		(*target)->flags |= STOPPED;
		//if (is_leader(g_session, *target))
		update_session_history_v2(get_group(*target));
	}
	if (PRINT_DEBUG)
		ft_dprintf(2, "[UPDATE BACKGROUND][WSTATUS AT THE END = \'%d\']\n", (*target)->wstatus);
	//ft_dprintf(2, "[UPDATE V2--2]ACTIVE PROCESSES: %p\n", (*target));
	//ft_dprintf(2, "------> %d\n", (*target)->flags);
}

bool            update_session_history(t_process *update)
{
    t_process*  cp_update;
	t_process*	fill;

	if (PRINT_DEBUG)
		ft_dprintf(2, "[UPDATE SESSION HISTORY (make a copy) ][\'%p\']\n", update);
    if (!(cp_update = process_new(update->pid, update->wstatus, update->data)))
		return (false);
	fill = g_session->history;
	g_session->history = cp_update;
	cp_update->next = fill;
	if (fill)
		fill->prev = cp_update;
	return (true);
}

bool			update_session_history_v2(t_group* update)
{
	t_history*	fill;
	t_history*	hist;

	if (!update || !(hist = ft_calloc(1, sizeof(t_history))))
		return (false);
	*hist = (t_history){.group=update};
	fill = g_session->hist;
	g_session->hist = hist;
	hist->next = fill;
	if (PRINT_DEBUG)
		ft_dprintf(2, "[UPDATE SESSION HISTORY][CURR GROUP IS: \'%p\'][ITS LEADER: \'%p\'][ %d ]\n", g_session->hist->group, g_session->hist->group->nil->next, g_session->hist->group->nil->next->pid);
	return (true);
}

t_process**		background_find(t_process* target, const char* search_type, t_group* group)
{
	const char*	modes[2] = { "PID", "STA" };
	int 		i;
	t_process*	remember = group->active_processes;

	i = 0;
	while (group->active_processes != group->nil)
	{
		while (i < 2 && ft_strncmp(modes[i], search_type, 3))
			i++;
		if (!i && target->pid == group->active_processes->pid)
		{
			group->active_processes = remember;
			return (&group->active_processes);
		}
		else if (i && target->wstatus == group->active_processes->wstatus)
		{
			group->active_processes = remember;
			return (&group->active_processes);
		}
		group->active_processes = group->active_processes->next;
	}
	group->active_processes = remember;
	return (NULL);
}

/*
t_process**		background_find_leader(t_process* target, const char* search_type, t_group* group)
{
	const char*	modes[2] = { "PID", "STA" };
	int 		i;
	t_process** leader;

	i = 0;
	leader = &group->nil->next;
	while (group->active_processes != group->nil)
	{
		while (i < 2 && ft_strncmp(modes[i], search_type, 3))
			i++;
		if (!i && target->pid == group->active_processes->pid)
			return (leader);
		else if (i && target->wstatus == group->active_processes->wstatus)
			return (leader);
		group->active_processes = group->active_processes->next;
	}
	return (NULL);
}
}
*/

bool		is_active_group(t_group* target)
{
	t_process*	process;

	if (group_empty(target))
		return (false);
	process = target->nil->next;
	if (PRINT_DEBUG)
		ft_dprintf(2, "[IS ACTIVE GROUP (leader): %p]\n", process);
	while (process != target->nil)
	{
		if (process->flags & (BACKGROUND | STOPPED))
			return (true);
		process = process->next;
	}
	return (false);
}

pid_t			get_process_leader_pid(t_group* nil, t_process* target)
{
	t_group*	groups;

	groups = nil->next;
	while (groups && groups != nil)
	{
		if (background_find(target, "PID", groups))
			return (groups->nil->next->pid);
		groups = groups->next;
	}
	return (0);
}

size_t			get_background_index(t_group* nil, t_process* target)
{
	size_t		index;
	t_process*	fill;
	t_group*	groups;
	t_group*	remember;

	index = 0;
	groups = nil->prev;
	remember = groups;
	while (groups != nil && (++index))
	{
		fill = groups->nil->prev;
		while (fill != groups->nil)
		{
			if (fill == target)
			{
				nil->prev = remember;
				return (index);
			}
			fill = fill->prev;
		}
		groups = groups->prev;
	}
	nil->prev = remember;
	return (index);
}

/*

void			force_exit_background(t_session* session)
{
	t_process**	fill;

	while (session->groups != session->nil)
	{
		while (session->groups->active_processes != session->groups->nil)
		{
			// TO DO: for SIGTTIN SIGTTOU -> SIGHUB
			kill(session->groups->active_processes->pid, SIGCONT);
			// wait for the process to exit and then iterate
			while (waitpid(session->groups->active_processes->pid, &session->groups->active_processes->wstatus, 0) <= 0)
				;
			if (WIFEXITED(session->groups->active_processes->wstatus))
			{
				fill = &session->groups->active_processes;
				session->groups->active_processes = session->groups->active_processes->next;
				remove_process(fill);
			}
			session->groups->active_processes = session->groups->active_processes->next;
		}
		session->groups = session->groups->next;
	}
}
*/

void			force_exit_background()
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = g_session->groups;

	while (g_session->groups != g_session->nil)
	{
		remember_leader = g_session->groups->active_processes;
		while (g_session->groups->active_processes != g_session->groups->nil)
		{
			// WORKS! The zombies catcher print it return status
			if (PRINT_DEBUG)
				ft_dprintf(2, "[FORCE EXIT][PROCESS: [PROCESS: \'%d\'][\'%p\']\n", g_session->groups->active_processes->pid, g_session->groups->active_processes);
			if (!(g_session->groups->active_processes->flags & NO_HANGUP)) // disown -h
				kill(g_session->groups->active_processes->pid, SIGHUP);
			if (g_session->groups->active_processes->flags & STOPPED) // check for disown -h if i have to continue too
				kill(g_session->groups->active_processes->pid, SIGCONT);
			//while (waitpid(g_session->groups->active_processes->pid, &g_session->groups->active_processes->wstatus, WUNTRACED) <= 0)
			//	;
			//if (WIFEXITED(g_session->groups->active_processes->wstatus))
			g_session->groups->active_processes = g_session->groups->active_processes->next;
			//else
			//	ft_dprintf(2, "[FORCE EXIT][PROCESS: \'%d\'][\'%p\'][DOESN'T EXIT!]\n", g_session->groups->active_processes->pid, g_session->groups->active_processes);
			
		}
		g_session->groups = g_session->groups->next;	
	}
	g_session->groups = remember;
}

bool			is_leader(t_process* target)
{
	t_group*	groups;

	if (!target)
		return (false);
	groups = g_session->groups;
	while (groups != g_session->nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->pid == target->pid)
			return (true);
		groups = groups->next;
	}
	return (false);
}

bool			is_not_ambigous(t_process* target)
{
	t_group*	groups;
	size_t		count;

	count = 0;
	if (!target)
		return (false);
	groups = g_session->groups;
	while (groups != g_session->nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->data && !ft_strncmp(groups->nil->next->data[0], target->data[0], ft_strlen(target->data[0])))
			count++;
		groups = groups->next;
	}
	return (count == 1);
}

void		remove_history_node(t_group* target)
{
	t_history*	prev;
	t_history*	first;

	first = g_session->hist;
	prev = NULL;
	while (g_session->hist)
	{
		if (target->nil->next->pid == g_session->hist->group->nil->next->pid)
		{
			if (prev)
				prev->next = g_session->hist->next;
			if (PRINT_DEBUG)
				ft_dprintf(2, "[RM HISTORY NODE][\'%p\'][ %d ]\n", g_session->hist, g_session->hist->group->nil->next->pid);
			free(g_session->hist);
			g_session->hist = NULL;
			break ;
		}
		prev = g_session->hist;
		g_session->hist = g_session->hist->next;
	}
	// update first
	if (g_session->hist && first->group->nil->next->pid == target->nil->next->pid)
		g_session->hist = first->next;
	else if (!g_session->hist)
		g_session->hist = NULL;
	else
		g_session->hist = first;
	if (PRINT_DEBUG && g_session->hist)
		ft_dprintf(2, "[RM HISTORY NODE][NOW CURR HISTORY NODE IS][\'%p\'][ %d ]\n", g_session->hist, g_session->hist->group->nil->next->pid);
}

void		remove_exited_zombies()
{
	t_group*	remember;
	t_group*	next;

	remember = g_session->groups;
	while (g_session->groups != g_session->nil)
	{
		next = g_session->groups->next;
		if (!is_active_group(g_session->groups))
		{
			remove_history_node(g_session->groups);
			if (PRINT_DEBUG)
				ft_dprintf(2, "[REMOVE EXITED ZOMBIES][REMOVE EXITED ZOMBIE GROUP: %p]\n", g_session->groups);
			group_remove_v2(&g_session->groups);
		}
		g_session->groups = next;
	}
	g_session->groups = remember;
}

bool		is_active_background()
{
	t_group*	remember;

	remember = g_session->groups;
	while (g_session->groups != g_session->nil)
	{
		if (is_active_group(g_session->groups))
		{
			g_session->groups = remember;
			return (true);
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
	return (false);
}

void		handle_exit_with_active_background(int exit_status)
{
	if (g_session->exit_count++ == 2 || !is_active_background())
		exit(exit_status);
	else
		write(STDERR_FILENO, "There are stopped jobs.\n", 25);
}

void		update_exit_count(const char* name)
{
	if (!ft_strncmp(name, "exit", 5) && g_session->exit_count == 1)
	{
		ft_dprintf(2, "TRUEEEEEEEEEEEEEEEEEEE\n");
		g_session->exit_count++;
	}
	else
		g_session->exit_count = 0;
}