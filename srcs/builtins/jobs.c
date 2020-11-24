/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 12:03:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/24 23:12:03 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signals.h>


/*
TESTS:

USAGE:
1) jobs from stopped job DONE
3) jobs from exit DONE (execpt for background processes) <-
4) jobs from pipe cmd DONE

ERRORS:
1) invalid flag + empty/ not empty background  -> error msg
2) invalid jobspec not empty/ not empty background -> error msg


*/

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
const char*			is_in_history(t_process* target)
{
	if (!g_session->hist) // for the momment
		return (" ");
	if (background_find(target, "PID", g_session->hist->group) && is_leader(target))
		return ("+");
	else if (g_session->hist->next && background_find(target, "PID", g_session->hist->next->group) && is_leader(target))
		return ("-");
	else
		return (" ");
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
void			print_process(int fd, t_process* target, int flags)
{
	char*		itoa1;
	char*		itoa2;
	int			i;

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
	print_signal(fd, target, flags & 8);
	//ft_dprintf(2, "PRINTS: %p\n", target);
	// [group index if leader][history index][space(s)][pid if -l][status][spaces][av]
	/*ft_dprintf(STDERR_FILENO, "%s%s%s%s %s %-19s",
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
*/
}

void			print_group(int fd, t_process* leader, int flags, t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = g_session->groups;

	if (PRINT_DEBUG) {
	ft_dprintf(2, "[PRINT GROUP]LEADER: %p\n", leader);
	ft_dprintf(2, "[PRINT GROUP]ITSELF PARAM: %p\n", itself);}
	// skip itself
	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;
	if (PRINT_DEBUG) {
	ft_dprintf(2, "[PRINT GROUP][g_session GROUP: %p]\n", g_session->groups);}
	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				print_process(fd, g_session->groups->active_processes, flags);
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			g_session->groups->active_processes = remember_leader;
			g_session->groups = remember;
			return ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
}

void			print_all_leaders(int fd, int flags)
{
	t_group*	remember;

	remember = g_session->groups;

	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next)
	{
		// TO DO: check in all the list i think
		if (!(flags & 2 && (WIFEXITED(g_session->groups->nil->next->wstatus) || WIFSTOPPED(g_session->groups->nil->next->wstatus))) 
				&& !(flags & 4 && (WIFEXITED(g_session->groups->nil->next->wstatus) || !WIFSTOPPED(g_session->groups->nil->next->wstatus))))
			print_signal(fd, g_session->groups->nil->next, 0);
		g_session->groups = g_session->groups->prev;
	}
	g_session->groups = remember;
}

void			print_all_groups(int fd, int flags)
{
	t_group*	remember;

	remember = g_session->groups;
	// First is last
	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next)
	{
		//ft_dprintf(2, "[PRINT ALL GROUPS][CURR GROUP IS: %p]\n", g_session->groups);
		print_group(fd, g_session->groups->nil->next, flags, remember);
		//ft_dprintf(2, "[PRINT ALL GROUPS][CURR GROUP IS: %p]\n", g_session->groups);
		g_session->groups = g_session->groups->prev;
		//ft_dprintf(2, "[PRINT ALL GROUPS][CURR GROUP IS: %p]\n", g_session->groups);
		//return ;
	}
	g_session->groups = remember;
}

int				ft_jobs(t_exec* args, t_term* term)
{
	int			flags;
	int			i;
	int			nb;
	t_process**	target;
	(void)term;

	flags = 0;
	i = -1;
	if ((flags = parse_flags(args->ac, &args->av[1], "nrsl", &nb)) < 0 && args->av[nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: jobs: %s: invalid option\n%s", args->av[1] ,"jobs: usage: jobs: [-lnprs] [jobspec ...] or jobs -x command [args]\n");
		return (CMD_BAD_USE);
	}
	//ft_dprintf(2, "PARSE FLAGS AFTER CHECK: %s\n", args->av[nb + 1]);
	if (session_empty() || g_session->groups->next == g_session->nil)
	{
		if (args->ac > 1 && flags < 0)
		{
			ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", args->av[args->ac]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	if (args->ac > 1)
	{
		//ft_dprintf(2, "NB FLAGS IS: %d\n", nb);
		if (args->ac > nb + 1) // check if theres some jobspec after and print it
		{
			//ft_dprintf(2, "[JOBSPEC IT CHECK: %d]\n", args->ac - nb - 1);
			while (++i < args->ac - nb - 1)
			{
				if (PRINT_DEBUG)
					ft_dprintf(2, "flags : %d\n", flags);
				if (!(target = jobspec_parser(args->ac, &args->av[nb + i], NULL)))
				{
					ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", args->av[nb + i + 1]);
					return (STD_ERROR);
				}
				if (PRINT_DEBUG)
					ft_dprintf(2, "[JOBS JOBSPEC][\'%p\']\n", *target);
				// print it here and apply flags
				flags = flags < 0 ? -flags : flags;
				if (flags > 0 && flags & 8)
					print_group(args->fds[1], *target, flags < 0 ? 0 : flags, g_session->groups);
				else if (!(flags > 0 && flags & 2 && (WIFEXITED((*target)->wstatus) || WIFSTOPPED((*target)->wstatus))) 
					&& !(flags > 0 && flags & 4 && (WIFEXITED((*target)->wstatus) || !WIFSTOPPED((*target)->wstatus))))
					print_signal(args->fds[1], *target, 0);
			}
			return (SUCCESS);
		}
	}
	// flags with no args or no args print all jobs in background
	if (flags > 0 && flags & 8)
		print_all_groups(args->fds[1] ,flags < 0 ? 0 : flags);
	else
		print_all_leaders(args->fds[1], flags < 0 ? 0 : flags);
	return (SUCCESS);
}