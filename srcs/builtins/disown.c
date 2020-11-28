/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:48:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 03:51:14 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

/* TESTS

1) Invalid flag DONE
2) Invalid jobspec DONE
3) Disown a running process DONE
4) Disown a stopped process DONE
5) Disown -a to multiple running pipes DONE
6) Disown -a to multiple stopped pipes DONE
7) Disown -r to stopped process + OTHER BUILTINS INTERACTIONS DONE
8) Disown -h // put the flag only for the momment

*/

// -r disown running  only + own = FALSE and is deleted from the history for STTOPED
// -a  disown all + rm from data-structure
// -h put a flag in process to for the momment and doesnt disown
// no flags disown curr
// parse flags like jobs

// disown seems not SIGCONT to stopped
// is just rm from the table so

/*
void			history_pop_front()
{
	t_process*	fill;

	if (g_session->history)
	{
		fill = g_session->history->next;
		if (PRINT_DEBUG)
			ft_dprintf(2, "[DISOWN][REMOVE FROM HISTORY: %p]\n", g_session->history);
		//delete_process(&g_session->history);
		free(g_session->history);
		g_session->history = fill;
	}
}
*/

void			history_pop_front_v2()
{
	t_history*	fill;

	if (g_session->hist)
	{
		fill = g_session->hist->next;
		free(g_session->hist);
		g_session->hist = fill;
	}
}



void			disown_process(t_process** target, int flags)
{
	int i;

	i = -1;
	if (flags & 1 && (*target)->flags & STOPPED) // -r only running processes
	{
		// put flag
		if (PRINT_DEBUG) {
		ft_dprintf(2, "[DISOWN][RESTRIC OPERATION TO: %p]\n", *target);}
		(*target)->flags |= RESTRICT_OP;
		
		// rm from history (probally this pop front needs a condition for flags + jobspec)
		history_pop_front();
		return ;
	}
	if (flags & 4) // -h process doenst recive SIGHUB when the term exits
	{
		(*target)->flags |= NO_HANGUP;
		return ;
	}

	if (PRINT_DEBUG) {
	ft_dprintf(2, "[DISOWN][flags: %d][\'%p\']\n", (*target)->flags, *target);}
	if ((*target)->flags & STOPPED)
		ft_dprintf(STDERR_FILENO, "minish: warning: deleting stopped job %lu with process group %d\n", background_index_get(g_session->nil, *target), process_get_leader_pid(g_session->nil, *target));
	remove_process(target);
}

/*
void		disown_all_processes(t_session* session)
{
	while (session->groups != session->nil)
	{
		while (42)
		{
			disown_process(session, &session->groups->active_processes, 0);
			if (session->groups->active_processes && session->groups->active_processes != session->groups->nil)
				session->groups->active_processes = session->groups->active_processes->next;
			else
				break ;
		}
		session->groups = session->groups->next;
	}
}
*/

void		disown_group(t_process* leader, int flags, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;
	t_process*	next;

	remember = g_session->groups;

	// skip itself
	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;

	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			if (PRINT_DEBUG)
				ft_dprintf(2, "[DISOWN][LEADER: %p][PID: %d]\n[DISOWN][CURR GROUP: %p][CURR ACTIVE PROCESSES: %p]\n", leader, leader->pid, g_session->groups, g_session->groups->active_processes);
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				if (PRINT_DEBUG)
					ft_dprintf(2, "[DISOWN][PROCESS: %d][\'%p\']\n", g_session->groups->active_processes->pid, g_session->groups->active_processes);
				next = g_session->groups->active_processes->next;
				disown_process(&g_session->groups->active_processes, flags);
				g_session->groups->active_processes = next;
			}
			if (!group_condition(g_session->groups, is_active))
			{
				history_session_remove_node(g_session->groups);
				t_group*	fill = g_session->groups;
				
				g_session->groups->prev->next = g_session->groups->next;
				g_session->groups->next->prev = g_session->groups->prev;
				free(fill);
				fill = NULL;
			}
			else
				g_session->groups->active_processes = remember_leader;
			g_session->groups = remember;
			return ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
}

void		disown_all_groups(int flags)
{
	t_group*	remember;
	t_group*	prev;

	remember = g_session->groups;

	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next)
	{
		prev = g_session->groups->prev;
		if (PRINT_DEBUG)
			ft_dprintf(2, "[DISOWN ALL GROUPS][CURR GROUP IS: %p][ACTIVE PROCESSES: %p]\n", g_session->groups, g_session->groups->active_processes);
		disown_group(g_session->groups->nil->next, flags, remember);
		g_session->groups = prev;
	}
	g_session->groups = remember;
}

/*
// put str pos arg as parse_flags arg for the norme and use this for jobs and disown
static int				parse_flags(int ac, const char* av)
{
	int			cont;
	int			flags;
	int 		i;

	cont = -1;
	flags = 0;
	while (++cont < ac - 1)
	{
		i = -1;
		while (av[++i])
		{
			if (av[i] == '-')
			{
				if (i == 0)
					continue ;
				else
					return (-1);
			}
			if ((flags |= (1 << ft_strpos("rah", av[i]))) < 0)
				return (-1);
		}
	}
	return (flags);
}
*/

int		ft_disown(t_exec* args, t_term* term)
{
	int	flags;
	t_process** target;
	int i;
	(void)term;
	int nb;

	flags = 0;
	i = -1;
	target = NULL;
	if ((flags = parse_flags(args->ac, &args->av[1], "rah", &nb)) < 0 && args->av[nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s", "minish: usage: disown: [-h] [-ar] [jobspec ... | pid ...]\n");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session->groups->next == g_session->nil)
	{
		if (args->ac > 1 && flags < 0)
		{
			ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", args->av[1]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	if (args->ac > 1)
	{
		ft_dprintf(2, "FLAGS ----> %d\n", flags);
		if (flags < 0 || !(flags & 2)) // not have to disown all
		{
			// disown jobspecs
			//if (PRINT_DEBUG)
				ft_dprintf(2, "[DISOWN JOBSPEC: flags: %d --- %d]\n", flags, args->ac - nb - 1);
			while (++i < args->ac - nb - 1)
			{
				if (!(target = jobspec_parser(args->ac, &args->av[nb + i], NULL)))
				{
					ft_dprintf(STDERR_FILENO, "minish: disown: %s: no such job\n", args->av[nb + i + 1]);
					return (STD_ERROR);
				}
				flags = flags < 0 ? -flags : flags;
				disown_group(*target, flags < 0 ? 0 : flags, g_session->groups);
			}
		}
		else
			disown_all_groups(flags < 0 ? 0 : flags);
	}
	else if (g_session->hist)
	{
		//target = background_find(g_session->history, "PID", g_session->groups);
		disown_group(g_session->hist->group->nil->next, flags < 0 ? 0 : flags, g_session->groups);
	}
	// disown curr
	return (SUCCESS);
}