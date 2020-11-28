/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 12:03:23 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 06:27:40 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signals.h>

void			print_process(int fd, t_process* target, int flags)
{
	char*		itoa1;
	char*		itoa2;
	int			i;

	i = -1;
	itoa1 = NULL;
	itoa2 = NULL;
	if (!target)
		return ;
	if (flags & 2 && (WIFEXITED(target->wstatus) \
			|| WIFSTOPPED(target->wstatus)))
		return ;
	if (flags & 4 && (WIFEXITED(target->wstatus) \
			|| !WIFSTOPPED(target->wstatus)))
		return ;
	print_signal(fd, target, flags & 8);
}

void			print_group(int fd, t_process* leader, int flags,
		t_group* itself)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = g_session->groups;
	if (g_session->groups == itself)
		g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes \
				!= g_session->groups->nil)
			{
				print_process(fd, g_session->groups->active_processes, flags);
				g_session->groups->active_processes = \
					g_session->groups->active_processes->next;
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
		if (!(flags & 2 && (WIFEXITED(g_session->groups->nil->next->wstatus) \
				|| WIFSTOPPED(g_session->groups->nil->next->wstatus))) 
				&& !(flags & 4 \
				&& (WIFEXITED(g_session->groups->nil->next->wstatus) \
				|| !WIFSTOPPED(g_session->groups->nil->next->wstatus))))
			print_signal(fd, g_session->groups->nil->next, STANDART);
		g_session->groups = g_session->groups->prev;
	}
	g_session->groups = remember;
}

void			print_all_groups(int fd, int flags)
{
	t_group*	remember;

	remember = g_session->groups;
	g_session->groups = g_session->nil->prev;
	while (g_session->groups != g_session->nil->next)
	{
		print_group(fd, g_session->groups->nil->next, flags, remember);
		g_session->groups = g_session->groups->prev;
	}
	g_session->groups = remember;
}

int			jobs_init_exeptions(t_exec *args, int *flags, int *nb)
{
	if ((*flags = parse_flags(args->ac, &args->av[1], "nrsl", nb)) < 0 \
			&& args->av[*nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: jobs: %s: invalid option\n%s", \
			args->av[1] ,"jobs: usage: jobs: [-lnprs] [jobspec ...] or jobs -x command [args]\n");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session->groups->next == g_session->nil)
	{
		if (args->ac > 1 && *flags < 0)
		{
			ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", \
				args->av[args->ac]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	return (42);
}

int				jobs_jobspec(t_exec *args, int nb, int flags)
{
	int			i;
	t_process	**target;

	i = -1;
	while (++i < args->ac - nb - 1)
	{
		if (!(target = jobspec_parser(args->ac, &args->av[nb + i], ignore_pid)))
		{
			ft_dprintf(STDERR_FILENO, "minish: jobs: %s: no such job\n", \
					args->av[nb + i + 1]);
			return (STD_ERROR);
		}
		if (flags & 8)
			print_group(args->fds[1], *target, flags, g_session->groups);
		else if (!(flags & 2 \
			&& (WIFEXITED((*target)->wstatus) || WIFSTOPPED((*target)->wstatus))) 
			&& !(flags & 4 && \
			(WIFEXITED((*target)->wstatus) || !WIFSTOPPED((*target)->wstatus))))
			print_signal(args->fds[1], *target, STANDART);
	}
	return (SUCCESS);
}

int				ft_jobs(t_exec* args)
{
	int			flags;
	int			exept;
	int			nb;

	flags = 0;
	if ((exept = jobs_init_exeptions(args, &flags, &nb)) != 42)
		return (exept);
	flags = flags < 0 ? -flags : flags;
	if (args->ac > 1 && args->ac > nb + 1)
		return (jobs_jobspec(args, nb, flags));
	if (flags & 8)
		print_all_groups(args->fds[1] ,flags);
	else
		print_all_leaders(args->fds[1], flags);
	return (SUCCESS);
}