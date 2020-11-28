/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 23:11:42 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 05:40:29 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <job_control.h>
#include <signals.h>

void	bg_core()
{
	if (g_session->groups->active_processes->flags & STOPPED)
	{
		g_session->groups->active_processes->flags &= ~STOPPED;
		g_session->groups->active_processes->flags |= BACKGROUND;
		kill(g_session->groups->active_processes->pid, SIGCONT);
		zombies_list_update(g_session->groups);
	}
}

void	resume_background_group(t_process* leader)
{
	t_group*	remember;
	t_process*	remember_leader;

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
				bg_core();
				g_session->groups->active_processes = \
					g_session->groups->active_processes->next;
			}
			g_session->groups->active_processes = remember_leader;
			break ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
}

int		bg_execptions(t_exec *args, t_process **target)
{
	if ((*target)->flags & RESTRICT_OP)
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
	}
	if ((*target)->flags & BACKGROUND)
	{
		ft_dprintf(STDERR_FILENO, "minish: job %lu already in background\n", \
				background_index_get(g_session->nil, *target));
		return (SUCCESS);
	}
	if ((*target)->flags & (SIGNALED | KILLED))
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: job has terminated\n");
		print_signal(STDERR_FILENO, *target, STANDART);
		return (STD_ERROR);
	}
	return (42);
}

int		ft_bg(t_exec* args)
{
    t_process**		target;
	int				exept;

	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: %s: invalid option\n%s\n", \
				args->av[1], "bg: usage: bg [job_spec]");
		return (CMD_BAD_USE);
	}
    if (session_empty() || g_session->groups->next == g_session->nil)
    {
        ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", \
				args->ac == 1 ? "current" : args->av[1]);
        return (STD_ERROR);
    }
	/* ------------------- another function -------------------------------------- */
	// TO DO: Change this like i did in kill 
	t_group*	remember;
	remember = g_session->groups;
	while (g_session->groups != g_session->nil->prev)
	{
    	target = g_session->groups->next != g_session->nil ? &g_session->groups->next->active_processes : &g_session->groups->active_processes;
		if ((*target)->flags & (BACKGROUND | EXITED))
			g_session->groups = g_session->groups->next;
		else
			break ;
	}
	g_session->groups = remember;
	/* ----------------------- end -------------------------------------------------*/
    if (args->ac > 1 && !(target = jobspec_parser(args->ac, args->av, ignore_pid)))
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->av[1]);
		return (STD_ERROR);
    }
	if ((exept = bg_execptions(args, target)) != 42)
		return (exept);
	print_index_args(*target);
	write(STDERR_FILENO, " &\n", 3);
	resume_background_group(*target);
    return (SUCCESS);
}
