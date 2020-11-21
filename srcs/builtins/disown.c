/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disown.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/19 18:48:29 by pablo             #+#    #+#             */
/*   Updated: 2020/11/19 18:53:43 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

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
		ft_dprintf(2, "[DISOWN][REMOVE FROM HISTORY: %p]\n", session->history);
		delete_process(&session->history);
		session->history = fill;
	}
}

void			disown_process(t_session* session, t_process** target, int flags)
{
	int i;

	i = -1;
	if (flags & 1 && !WIFEXITED((*target)->wstatus) && WIFSTOPPED((*target)->wstatus)) // -r only running processes
	{
		// put flag
		(*target)->flags |= RESTRICT_OP;
		
		// rm from history (probally this pop front needs a condition for flags + jobspec)
		history_pop_front(session);
		return ;
	}
	if (flags & 4) // -h process doenst recive SIGHUB when the term exits
	{
		(*target)->flags |= NO_HANGUP;
		return ;
	}
	while (++i < 2) // check 2 times: 1 for + and another for -
		if (session->history && session->history->pid == (*target)->pid)
			history_pop_front(session);

	if (!WIFEXITED((*target)->wstatus) && WIFSTOPPED((*target)->wstatus))
		ft_dprintf(STDERR_FILENO, "minish: warning: deleting stopped job %lu with process group %d\n", get_background_index(session->nil, *target), get_process_leader_pid(session->nil, *target));
	remove_process(target);
}

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

int		ft_disown(t_exec* args, t_term* term)
{
	int	flags;
	t_process** target;
	int i;

	flags = 0;
	i = -1;
	target = NULL;
	ft_dprintf(2, "[DISOWN]\n");
	if (!term->session->groups || term->session->groups == term->session->nil \
			|| !term->session->groups->active_processes \
			/*|| term->session->groups->active_processes == term->session->groups->nil*/)
		return (SUCCESS);
	if (args->ac > 1)
	{
		if ((flags = parse_flags(args->ac, args->av[1])) < 0 && ft_dprintf(2, "[Flags are: %d]\n", flags) &&args->av[1][0] == '-')
		{
			ft_dprintf(STDERR_FILENO, "%s", "minish: usage: disown: [-h] [-ar] [jobspec ... | pid ...]\n");
			return (CMD_BAD_USE);
		}
		else if (flags < 0 || !(flags & 2)) // not have to disown all
		{
			// disown jobspecs
			ft_dprintf(2, "[DISOWN JOBSPEC]\n");

			while (++i < args->ac - (flags > 0 ? 2 : 1))
			{
				if (!(target = jobspec_parser(term->session, args->ac, &args->av[flags > 0 ? 1 : 0], NULL)))
				{
					ft_dprintf(STDERR_FILENO, "minish: disown: %s: no such job\n", args->av[2]);
					return (STD_ERROR);
				}
				disown_process(term->session, target, flags > 0 ? 1 : 0);
			}
		}
		else
			disown_all_processes(term->session);
	}
	if (term->session->history)
	{
		// TO DO: disown all the group not only the leader
		target = background_find(term->session->history, "PID", term->session->groups);
		disown_process(term->session, target, flags > 0 ? 1 : 0);
	}
	// disown curr
	return (SUCCESS);
}