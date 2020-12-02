/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 09:26:12 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <job_control.h>
#include <signals_print.h>

static bool		fg_delete(void)
{
	t_group		*next;
	if (!group_condition(g_session.groups, is_active))
	{
		zombies_list_remove_node(g_session.groups);
		history_session_remove_node(g_session.groups);
		(void)next;
		/*
		next = g_session.groups->next;
		g_session.groups->prev->next = next;
		next->prev = g_session.groups->prev;
		delete_group_input(&g_session.groups);
		delete_processes(g_session.groups);
		free(g_session.groups);
		g_session.groups = next;
		*/
		group_remove(&g_session.groups);
		return (true);
	}
	return (false);
}

static int		fg_core(void)
{
	if (g_session.groups->active_processes->flags & (STOPPED | BACKGROUND))
	{
		kill(g_session.groups->active_processes->pid, SIGCONT);
		background_update(&g_session.groups->active_processes);
		if (!(g_session.groups->active_processes->wstatus & STOPPED))
			return (g_session.groups->active_processes->ret);
	}
	return (SUCCESS);
}

static int		fg_init_exceptions(t_exec *args)
{
	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s: fg: %s: invalid option\n%s\n", \
			g_session.name, args->av[1], "fg: usage: fg [job_spec]");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session.groups->next == g_session.nil)
	{
		ft_dprintf(STDERR_FILENO, "%s: fg: %s: no such job\n", \
		g_session.name, args->ac == 1 ? "current" : args->av[1]);
		return (STD_ERROR);
	}
	return (42);
}

// TODO: Use g_session name for minish

static int		fg_exeptions(t_exec *args, t_process **target)
{
	if ((*target)->flags & RESTRICT_OP)
	{
		ft_dprintf(STDERR_FILENO, "%s: fg: %s: no such job\n", \
			g_session.name, args->av[1]);
		return (STD_ERROR);
	}
	if ((*target)->flags & (SIGNALED | KILLED))
	{
		ft_dprintf(STDERR_FILENO, "%s: fg: job has terminated\n", \
			g_session.name);
		print_signal(STDERR_FILENO, *target, STANDART);
		return (STD_ERROR);
	}
	return (42);
}

int				b_fg(t_exec *args)
{
	t_process	**target;
	int			exept;

	if ((exept = fg_init_exceptions(args)) != 42)
		return (exept);
	target = g_session.groups->active_processes == g_session.groups->nil \
		? &g_session.groups->next->active_processes \
		: &g_session.groups->active_processes;
	if (args->ac > 1 && !(target = jobspec_parser(args->ac, args->av, \
			ignore_pid)))
	{
		ft_dprintf(STDERR_FILENO, "%s: fg: %s: no such job\n", \
			g_session.name, args->av[1]);
		return (STD_ERROR);
	}
	if ((exept = fg_exeptions(args, target)) != 42)
		return (exept);
	print_job_args(STDERR_FILENO, *target);
	write(STDERR_FILENO, "\n", 1);
	return (for_each_in_group(*target, fg_core, fg_delete));
}
