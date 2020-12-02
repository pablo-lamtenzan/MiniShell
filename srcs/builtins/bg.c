/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 23:11:42 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 09:14:32 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <job_control.h>
#include <signals_print.h>

static int			bg_core(void)
{
	if (g_session.groups->active_processes->flags & STOPPED)
	{
		//signal(SIGCHLD, SIG_DFL);
		g_session.groups->active_processes->flags &= ~STOPPED;
		g_session.groups->active_processes->flags |= BACKGROUND;
		//ft_dprintf(2, "[BG][KILL -SIGCONT to group (\'%p\')][pid = %d]\n", g_session.groups, g_session.groups->active_processes->pid);
		kill(g_session.groups->active_processes->pid, SIGCONT);
		zombies_list_update(g_session.groups);
		//signal(SIGCHLD, zombies_catcher);
	}
	return (SUCCESS);
}

static int			bg_execptions(t_exec *args, t_process **target)
{
	if ((*target)->flags & RESTRICT_OP)
	{
		ft_dprintf(STDERR_FILENO, "%s: bg: %s: no such job\n", g_session.name, \
			args->av[1]);
		return (STD_ERROR);
	}
	if ((*target)->flags & BACKGROUND)
	{
		ft_dprintf(STDERR_FILENO, "%s: job %lu already in background\n", \
			g_session.name, background_index_get(g_session.nil, *target));
		return (SUCCESS);
	}
	if ((*target)->flags & (SIGNALED | KILLED))
	{
		ft_dprintf(STDERR_FILENO, "%s: bg: job has terminated\n",
			g_session.name);
		print_signal(STDERR_FILENO, *target, STANDART);
		return (STD_ERROR);
	}
	return (42);
}

static int			bg_skip(t_process ***target)
{
	t_group			*remember;

	remember = g_session.groups;
	while (g_session.groups != g_session.nil->prev)
	{
		*target = g_session.groups->next != g_session.nil ? \
			&g_session.groups->next->active_processes \
			: &g_session.groups->active_processes;
		if ((**target)->flags & (BACKGROUND | EXITED))
			g_session.groups = g_session.groups->next;
		else
			break ;
	}
	g_session.groups = remember;
	return (true);
}

static int			bg_init_exeption(t_exec *args)
{
	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s: bg: %s: invalid option\n%s\n", \
			g_session.name ,args->av[1], "bg: usage: bg [job_spec]");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session.groups->next == g_session.nil)
	{
		ft_dprintf(STDERR_FILENO, "%s: bg: %s: no such job\n", \
			g_session.name, args->ac == 1 ? "current" : args->av[1]);
		return (STD_ERROR);
	}
	return (42);
}

int					b_bg(t_exec *args)
{
    t_process		**target;
	int				exept;

	if ((exept = bg_init_exeption(args)) != 42)
		return (exept);
	if (bg_skip(&target) && args->ac > 1 \
		&& !(target = jobspec_parser(args->ac, args->av, ignore_pid)))
	{
		ft_dprintf(STDERR_FILENO, "%s: bg: %s: no such job\n", \
			g_session.name, args->av[1]);
		return (STD_ERROR);
	}
	if ((exept = bg_execptions(args, target)) != 42)
		return (exept);
	print_index_args(*target);
	write(STDERR_FILENO, " &\n", 3);
	return (for_each_in_group(*target, bg_core, NULL));
}
