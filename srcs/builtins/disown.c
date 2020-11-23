/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:48:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 07:45:07 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

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


void			history_pop_front(t_session* session)
{
	t_process*	fill;

	if (session->history)
	{
		fill = session->history->next;
		if (PRINT_DEBUG)
			ft_dprintf(2, "[DISOWN][REMOVE FROM HISTORY: %p]\n", session->history);
		//delete_process(&session->history);
		free(session->history);
		session->history = fill;
	}
}

void			history_pop_front_v2(t_session* session)
{
	t_history*	fill;

	if (session->hist)
	{
		fill = session->hist->next;
		free(session->hist);
		session->hist = fill;
	}
}

void			remove_process(t_process** target)
{
	t_process**	next;
	t_process**	prev;

	next = &(*target)->next;
	prev = &(*target)->prev;
	if (PRINT_DEBUG) {
	ft_dprintf(2, "[REMOVE PROCESS][NEXT = \'%p\']\n", (*next));
	ft_dprintf(2, "[REMOVE PROCESS][PREV = \'%p\']\n", (*prev));
	}
	(*next)->prev = *prev;
	(*prev)->next = *next;
	if (PRINT_DEBUG) {
	ft_dprintf(2, "[REMOVE: \'%p\']\n", *target);
	}
	free(*target); // not only free
	*target = NULL;
}

void			disown_process(t_session* session, t_process** target, int flags)
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
		history_pop_front_v2(session);
		return ;
	}
	if (flags & 4) // -h process doenst recive SIGHUB when the term exits
	{
		(*target)->flags |= NO_HANGUP;
		return ;
	}
	while (++i < 2) // check 2 times: 1 for + and another for -
		if (session->hist && background_find(*target, "PID", session->hist->group))
			history_pop_front_v2(session);
	if (PRINT_DEBUG) {
	ft_dprintf(2, "[DISOWN][flags: %d][\'%p\']\n", (*target)->flags, *target);}
	if ((*target)->flags & STOPPED)
		ft_dprintf(STDERR_FILENO, "minish: warning: deleting stopped job %lu with process group %d\n", get_background_index(session->nil, *target), get_process_leader_pid(session->nil, *target));
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

void		disown_group(t_session* session, t_process* leader, int flags, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;
	t_process*	next;

	remember = session->groups;

	// skip itself
	if (session->groups == itself)
		session->groups = session->groups->next;

	while (session->groups != session->nil)
	{
		if (session->groups->nil->next->pid == leader->pid)
		{
			if (PRINT_DEBUG)
				ft_dprintf(2, "[DISOWN][LEADER: %p][PID: %d]\n[DISOWN][CURR GROUP: %p][CURR ACTIVE PROCESSES: %p]\n", leader, leader->pid, session->groups, session->groups->active_processes);
			remember_leader = session->groups->active_processes;
			while (session->groups->active_processes != session->groups->nil)
			{
				if (PRINT_DEBUG)
					ft_dprintf(2, "[DISOWN][PROCESS: %d][\'%p\']\n", session->groups->active_processes->pid, session->groups->active_processes);
				next = session->groups->active_processes->next;
				disown_process(session, &session->groups->active_processes, flags);
				session->groups->active_processes = next;
			}
			if (!is_active_group(session->groups))
			{
				t_group*	fill = session->groups;
				session->groups->prev->next = session->groups->next;
				session->groups->next->prev = session->groups->prev;
				free(fill);
				fill = NULL;
			}
			else
				session->groups->active_processes = remember_leader;
			session->groups = remember;
			return ;
		}
		session->groups = session->groups->next;
	}
	session->groups = remember;
}

void		disown_all_groups(t_session* session, int flags)
{
	t_group*	remember;
	t_group*	prev;

	remember = session->groups;

	session->groups = session->nil->prev;
	while (session->groups != session->nil->next)
	{
		prev = session->groups->prev;
		if (PRINT_DEBUG)
			ft_dprintf(2, "[DISOWN ALL GROUPS][CURR GROUP IS: %p][ACTIVE PROCESSES: %p]\n", session->groups, session->groups->active_processes);
		disown_group(session, session->groups->nil->next, flags, remember);
		session->groups = prev;
	}
	session->groups = remember;
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

	flags = 0;
	i = -1;
	target = NULL;
	if ((flags = parse_flags(args->ac, args->av[1], "rah")) < 0 && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s", "minish: usage: disown: [-h] [-ar] [jobspec ... | pid ...]\n");
		return (CMD_BAD_USE);
	}
	if (session_empty(term->session) || term->session->groups->next == term->session->nil)
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
		if (flags < 0 || !(flags & 3)) // not have to disown all
		{
			// disown jobspecs
			if (PRINT_DEBUG)
				ft_dprintf(2, "[DISOWN JOBSPEC: flags: %d --- %d]\n", flags, args->ac - (flags > 0 ? 2 : 1));

			while (++i < args->ac - (flags > 0 ? 2 : 1))
			{
				if (!(target = jobspec_parser(term->session, args->ac, &args->av[flags > 0 ? 1 : 0], NULL)))
				{
					ft_dprintf(STDERR_FILENO, "minish: disown: %s: no such job\n", args->av[2]);
					return (STD_ERROR);
				}
				disown_group(term->session, *target, flags < 0 ? 0 : flags, term->session->groups);
			}
		}
		else
			disown_all_groups(term->session, flags < 0 ? 0 : flags);
	}
	else if (term->session->hist)
	{
		//target = background_find(term->session->history, "PID", term->session->groups);
		disown_group(term->session, term->session->hist->group->nil->next, flags < 0 ? 0 : flags, term->session->groups);
	}
	// disown curr
	return (SUCCESS);
}