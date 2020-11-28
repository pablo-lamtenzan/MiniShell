/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 23:58:24 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <job_control.h>
#include <signals.h>

bool			fg_delete()
{
	t_group		*next;
	if (!group_condition(g_session->groups, is_active))
	{
		zombies_list_remove_node(g_session->groups);
		history_session_remove_node(g_session->groups);
		next = g_session->groups->next;
		g_session->groups->prev->next = next;
		next->prev = g_session->groups->prev;
		free(g_session->groups);
		g_session->groups = next;
		return (true);
	}
	return (false);
}

int 	fg_core()
{
	if (g_session->groups->active_processes->flags & (STOPPED | BACKGROUND))
	{
		kill(g_session->groups->active_processes->pid, SIGCONT);
		background_update(&g_session->groups->active_processes);
		if (!(g_session->groups->active_processes->wstatus & STOPPED))
			return (g_session->groups->active_processes->ret);
	}
	return (SUCCESS);
}

// 26 lines fck
int		resume_group(t_process* leader)
{
	t_group*	remember;
	t_process*	remember_leader;
	int 		ret;

	ret = SUCCESS;
	remember = g_session->groups;
	g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		if (g_session->groups->active_processes->pid == leader->pid)
		{
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes \
				!= g_session->groups->nil)
			{
				ret = fg_core();
				g_session->groups->active_processes = \
					g_session->groups->active_processes->next;
			}
			if (!fg_delete())
				g_session->groups->active_processes = remember_leader;
			break ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
	return (ret);
}

int		fg_init_exceptions(t_exec *args)
{
	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: fg: %s: invalid option\n%s\n", \
				args->av[1], "fg: usage: fg [job_spec]");
		return (CMD_BAD_USE);
	}
    if (session_empty() || g_session->groups->next == g_session->nil)
    {
        ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", \
			args->ac == 1 ? "current" : args->av[1]);
        return (STD_ERROR);
    }
	return (42);
}

int		fg_exeptions(t_exec *args, t_process **target)
{
	if ((*target)->flags & RESTRICT_OP)
	{
		ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
	}
	if ((*target)->flags & (SIGNALED | KILLED))
	{
		ft_dprintf(STDERR_FILENO, "minish: fg: job has terminated\n");
		print_signal(STDERR_FILENO, *target, STANDART);
		return (STD_ERROR);
	}
	return (42);
}

int		ft_fg(t_exec* args)
{
    t_process**		target;
	int				exept;

	if ((exept = fg_init_exceptions(args)) != 42)
		return (exept);
    target = g_session->groups->active_processes == g_session->groups->nil \
		? &g_session->groups->next->active_processes \
		: &g_session->groups->active_processes;
	if (args->ac > 1 && !(target = jobspec_parser(args->ac, args->av, \
			ignore_pid)))
	{
		ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->av[1]);
		return (STD_ERROR);
    }
	if ((exept = fg_exeptions(args, target)) != 42)
		return (exept);
	print_job_args(STDERR_FILENO, *target);
	write(STDERR_FILENO, "\n", 1);
    return (resume_group(*target));
}