/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 12:03:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 19:44:40 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

/*	- Each [n] is a group index, only in leaders
	- "-n" is a mistery for the momment
	- "-r for running process only" WIFRUNNING
	- "-s" for stopped jobs "IFSTTOPED"
	- "-l" displays pid before status and after index
	

*/

/*
bool			is_leader(t_session* session, t_process* target)
{
	t_group*	groups;

	if (!target)
		return (false);
	groups = session->groups;
	while (groups != session->nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->pid == target->pid)
			return (true);
		groups = groups->next;
	}
	return (false);
}
*/
/*
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
			if ((flags |= (1 << ft_strpos("nrsl", av[i]))) < 0)
				return (-1);
		}
	}
	return (flags);
}
*/
const char*			is_in_history(t_session* session, t_process* target)
{
	if (!session->history) // for the momment
		return ("");
	if (target->pid == session->history->pid)
		return ("+");
	else if (session->history->next && target->pid == session->history->next->pid)
		return ("-");
	else
		return ("");
}

const char*			get_status(int wstatus)
{
	if (WIFSTOPPED(wstatus))
		return ("Stopped");
	if (WIFEXITED(wstatus))
		return ("Done");
	else if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGCONT)
		return ("Running");
	else
		return ("Have to handle it");
}

// -n flag no alrreaddy handled
void			print_process(t_session* session, t_process* target, int flags)
{
	char*		itoa1;
	char*		itoa2;
	int			i;
	const bool		leader = is_leader(session, target);

	i = -1;
	itoa1 = NULL;
	itoa2 = NULL;
	//ft_dprintf(2, "TEST: %p\n", target);
	if (!target)
		return ;
	if (flags & 2 && (WIFEXITED(target->wstatus) || WIFSTOPPED(target->wstatus))) // -r print runnig
		return ;
	if (flags & 4 && (WIFEXITED(target->wstatus) || !WIFSTOPPED(target->wstatus))) // -s print stopped
		return ;
	//ft_dprintf(2, "PRINTS: %p\n", target);
	// [group index if leader][history index][space(s)][pid if -l][status][spaces][av]
	ft_dprintf(STDERR_FILENO, "%s%s%s%s %s %-19s",
		leader ? "[" : "",
		leader ? itoa1 = ft_itoa(get_background_index(session->nil, target)) : "", // index
		leader ? "]" : "",
		is_in_history(session, target), // history index or espace
		flags & 8 ? itoa2 = ft_itoa(target->pid) : "", // pid
		get_status(target->wstatus) // status
	);
	while (target->data && target->data[++i])
		ft_dprintf(STDERR_FILENO, "%s%s", target->data[i], target->data[i + 1] ? " " : "");
	write(2, "\n", 1);
	free(itoa1);
	free(itoa2);
}

void			print_group(t_session* session, t_process* leader, int flags, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = session->groups;

	ft_dprintf(2, "[PRINT GROUP]LEADER: %p\n", leader);
	ft_dprintf(2, "[PRINT GROUP]ITSELF PARAM: %p\n", itself);
	// skip itself
	if (session->groups == itself)
		session->groups = session->groups->next;
	ft_dprintf(2, "[PRINT GROUP][SESSION GROUP: %p]\n", session->groups);
	while (session->groups != session->nil)
	{
		if (session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = session->groups->active_processes;
			while (session->groups->active_processes != session->groups->nil)
			{
				print_process(session, session->groups->active_processes, flags);
				session->groups->active_processes = session->groups->active_processes->next;
			}
			session->groups->active_processes = remember_leader;
			session->groups = remember;
			return ;
		}
		session->groups = session->groups->next;
	}
	session->groups = remember;
}

void			print_all_groups(t_session* session, int flags)
{
	t_group*	remember;

	remember = session->groups;
	// First is last
	session->groups = session->nil->prev;
	while (session->groups != session->nil->next)
	{
		//ft_dprintf(2, "[PRINT ALL GROUPS][CURR GROUP IS: %p]\n", session->groups);
		print_group(session, session->groups->nil->next, flags, remember);
		//ft_dprintf(2, "[PRINT ALL GROUPS][CURR GROUP IS: %p]\n", session->groups);
		session->groups = session->groups->prev;
		//ft_dprintf(2, "[PRINT ALL GROUPS][CURR GROUP IS: %p]\n", session->groups);
		//return ;
	}
	session->groups = remember;
}

// TO DO: correct index
int				ft_jobs(t_exec* args, t_term* term)
{
	int			flags;
	int			i;
	t_process**	target;

	flags = 0;
	i = -1;
	 //if (!term->session->groups || term->session->groups == term->session->nil 
		//	|| !term->session->groups->active_processes
			/*|| term->session->groups->active_processes == term->session->groups->nil)*/
	if (session_empty(term->session) || term->session->groups->next == term->session->nil)
		return (SUCCESS);
	if (args->ac > 1)
	{
		// get flags
		if ((flags = parse_flags(args->ac, args->av[1], "nrsl")) < 0 && args->av[1][0] == '-')
		{
			ft_dprintf(STDERR_FILENO, "%s", "minish: usage: jobs: [-lnprs] [jobspec ...] or jobs -x command [args]\n");
			return (CMD_BAD_USE);
		}
		if (args->ac >= 2) // check if theres some jobspec after and print it
		{
			while (++i < args->ac - (flags > 0 ? 2 : 1))
			{
				ft_dprintf(2, "flags : %d\n", flags);
				if (!(target = jobspec_parser(term->session, args->ac, &args->av[flags > 0 ? 1 : 0], NULL)))
				{
					ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", args->av[2]);
					return (STD_ERROR);
				}
				// print it here and apply flags
				print_group(term->session, *target, flags < 0 ? 0 : flags, term->session->groups);
				return (SUCCESS);
			}
			//return (SUCCESS);
		}
	}
	// flags with no args or no args print all jobs in background
	print_all_groups(term->session, flags < 0 ? 0 : flags);
	return (SUCCESS);
}