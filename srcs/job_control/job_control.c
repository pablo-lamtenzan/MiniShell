/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:39:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 04:57:42 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <signals.h>

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
		//(*target)->flags |= EXITED; only for zombies i guess
		(*target)->ret = WEXITSTATUS((*target)->wstatus);
	}
	// stopped
	else
	{
		//if (PRINT_DEBUG)
			ft_dprintf(2, "[PROCESS DOESN'T EXIT]\n");
		(*target)->flags |= STOPPED;
		//g_session->st = ; // CAN CNAGE DEPENDS OF THE SIGNAL
		update_session_history_v2(get_group(*target));
	}
	if (PRINT_DEBUG)
		ft_dprintf(2, "[UPDATE BACKGROUND][WSTATUS AT THE END = \'%d\']\n", (*target)->wstatus);
	//ft_dprintf(2, "[UPDATE V2--2]ACTIVE PROCESSES: %p\n", (*target));
	//ft_dprintf(2, "------> %d\n", (*target)->flags);
}
/*
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
*/

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

bool			update_zombies(t_group** update)
{
	t_background*	fill;
	t_background*	zombie;

	if (!update || !(zombie = ft_calloc(1, sizeof(t_background))))
		return (false);
	*zombie = (t_background){.background_group=update};
	fill = g_session->zombies;
	g_session->zombies = zombie;
	zombie->next = fill;
	//ft_dprintf(2, "UPDATE ZOMBIES: NEW NODE IN ZOMBIE LIST CONTANING THE ADDR: %p [\'%p\']\n", g_session->zombies, *update);
	return (true);
}

t_process**		background_find(t_process* target, const char* search_type, t_group* group)
{
	const char*	modes[2] = { "PID", "STA" };
	int 		i;
	//ft_dprintf(2, "[TEST: %p]\n", group);
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

bool		protect_process(t_group* target)
{
	t_process*	 process;
	if (group_empty(target))
		return (false);
	process = target->nil->next;
	while (process != target->nil)
	{
		if (process->flags & NO_DELETE)
			return (true);
		process = process->next;
	}
	return (false);
}

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
		if (process->flags & (BACKGROUND | STOPPED | SIGNALED))
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
			kill(g_session->groups->active_processes->pid, SIGCONT);
			if (!(g_session->groups->active_processes->pid & NO_HANGUP))
			{
				while (waitpid(g_session->groups->active_processes->pid, &g_session->groups->active_processes->wstatus, 0) <= 0)
					;
			}
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

bool			is_not_ambigous_v2(const char* niddle)
{
	t_group*	groups;
	int			count;
	int			error;

	count = -1;
	error = 0;
	groups = g_session->groups;
	while (groups != g_session->nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->data)
		{
			// TO DO: CHECK THE DOUBLE NIDLE IN SAME WORD
			while (++count < matrix_height((char**)groups->active_processes->data))
				if (ft_strnstr(groups->active_processes->data[count], niddle, ft_strlen(niddle)))
					error++;
		}
		groups = groups->next;
	}
	return (error == 1);
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

void		remove_zombie_node(t_group* target)
{
	t_background*	prev;
	t_background*	first;

	first = g_session->zombies;
	prev = NULL;
	if (!target)
		return ;
	while (g_session->zombies)
	{
		if (*g_session->zombies->background_group && target->nil->next->pid == (*g_session->zombies->background_group)->nil->next->pid)
		{
			if (prev)
				prev->next = g_session->zombies->next;
			//if (PRINT_DEBUG)
			if (first == g_session->zombies)
				first = NULL;
			//ft_dprintf(2, "[RM ZOMBIE NODE][\'%p\'][GROUP][\'%p\'][ %d ]\n", g_session->zombies, *g_session->zombies->background_group , (*g_session->zombies->background_group)->nil->next->pid);
			free(g_session->zombies);
			g_session->zombies = NULL;
			break ;
		}
		prev = g_session->zombies;
		g_session->zombies = g_session->zombies->next;
	}
	// update first
	if (first && *first->background_group && (*first->background_group)->nil->next->pid == target->nil->next->pid)
		g_session->zombies = first->next;
	else
		g_session->zombies = first;
	if (PRINT_DEBUG && g_session->zombies && *g_session->zombies->background_group)
		ft_dprintf(2, "[RM ZOMBIE NODE][NOW CURR HISTORY NODE IS][\'%p\'][ %d ]\n", g_session->zombies, (*g_session->zombies->background_group)->nil->next->pid);
	//ft_dprintf(2, "ZOMBIES ARE NOW: %p\n", g_session->zombies);
	//if (g_session->zombies && g_session->zombies->next)
	//	ft_dprintf(2, "ZOMBIES THE NEXT: %p\n", g_session->zombies);
}

void		remove_exited_zombies()
{
	t_group*	remember;
	t_group*	next;
	//bool		execption;

	//execption = true;
	remember = g_session->groups;
	while (g_session->groups && g_session->groups != g_session->nil)
	{
		next = g_session->groups->next;
		
		if (!is_active_group(g_session->groups) && !protect_process(g_session->groups))
		{
			if (g_session->groups == remember)
				remember = remember->next;
			//if (!remember || remember == g_session->nil)
			//	execption = true;
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
		g_session->exit_count++;
	else
		g_session->exit_count = 0;
}

void		get_group_return()
{
	t_process* remember;

	if (is_active_group(g_session->groups)) // take care about signaled here too
	{
		//
		//if (!(g_session->groups->nil->prev->flags & (STOPPED | BACKGROUND)))
		//	g_session->st = g_session->groups->nil->prev->ret;
		//else
		//{
			remember = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				if (g_session->groups->active_processes->flags & (STOPPED | BACKGROUND))
				{
					// First is stopped return the first return value: 128 + STOPPPED SIG
					if (g_session->groups->active_processes->prev == g_session->groups->nil)
						g_session->st = SIGNAL_BASE + WSTOPSIG(g_session->groups->active_processes->wstatus);
					// Else return 128 + the prev ret
					else
						g_session->st = SIGNAL_BASE + g_session->groups->active_processes->prev->ret;
					//ft_dprintf(2, "[CATCHET GROUP RET IS: %d]\n", g_session->st);
					g_session->groups->active_processes = remember;
					return ;
				}
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			g_session->groups->active_processes = remember;
		//}
	}
}

t_deadzombie*		endzombie_new(t_process** target)
{
	t_deadzombie*	deadzombie;
	
	if (!(deadzombie = ft_calloc(1, sizeof(t_deadzombie))))
		return (NULL);
	*deadzombie = (t_deadzombie){.deadzombie=target};
	return (deadzombie);
}

void		endzombie_push_back(t_deadzombie* target)
{
	t_deadzombie*	remember;

	if (!(remember = g_session->dead_zombies))
	{
		g_session->dead_zombies = target;
		return ;
	}
	while (g_session->dead_zombies->next)
		g_session->dead_zombies = g_session->dead_zombies->next;
	g_session->dead_zombies->next = target;
}

void		delete_endzombies()
{
	t_deadzombie*	fill;

	while (g_session->dead_zombies)
	{
		fill = g_session->dead_zombies->next;
		//ft_dprintf(2, "[DELETE ENDZOMBIES][DELETE NODE: %d]\n", (*g_session->end_zombies->endzombie)->pid);
		free(g_session->dead_zombies);
		g_session->dead_zombies = fill;
	}
	g_session->dead_zombies = NULL;
}

void		print_endzombies()
{
	t_deadzombie*	first;

	first = g_session->dead_zombies;
	while (g_session->dead_zombies)
	{
		(*g_session->dead_zombies->deadzombie)->flags &= ~NO_DELETE;
		print_signal(STDERR_FILENO, *g_session->dead_zombies->deadzombie, STANDART);
		g_session->dead_zombies = g_session->dead_zombies->next;
	}
	g_session->dead_zombies = first;
	delete_endzombies();
}

bool	group_exited(t_group* group)
{
	t_process* leader;

	leader = group->active_processes;
	while (leader != group->nil)
	{
		if (leader->flags & EXITED)
			return (true);
		leader = leader->next;
	}
	return (false);	
}

void	rm_exited_from_history()
{
	t_history* first;
	t_history*	next;

	first = g_session->hist;

	while (g_session->hist)
	{
		next = g_session->hist->next;
		if (group_exited(g_session->hist->group))
		{
			if (first == g_session->hist)
				first = first->next;
			free(g_session->hist);
		}
		g_session->hist = next;
	}
	g_session->hist = first;
}