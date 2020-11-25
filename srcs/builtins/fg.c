/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 20:39:31 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <job_control.h>
#include <signals.h>

/* TESTS

1) No background -> error msg
2) FG with flags / No background or background -> error msg
3) background + invalid jobspec -> error msg
4) background + curr -> all ok
5) background + valid jobspec -> all ok

*/

bool	ignore_pid(int ac, char*const* av)
{
	(void)ac;
	if (is_string_digit(av[1]))
		return (false);
	return (true);
}

int		resume_group(t_process* leader)
{
	t_group*	remember;
	t_process*	remember_leader;
	int 		ret;

	ret = SUCCESS;
	remember = g_session->groups;

	// skip itself
	if (PRINT_DEBUG)
		ft_dprintf(2, "[FG][SKIPPED ITSELF: %p]\n", remember);
	g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		if (PRINT_DEBUG)
			ft_dprintf(2, "test:::::: %p [%d] --- %p [%d] \n", g_session->groups->active_processes, g_session->groups->active_processes->pid, leader, leader->pid);
		if (g_session->groups->active_processes->pid == leader->pid)
		{
			if (PRINT_DEBUG)
				ft_dprintf(2, "[FG][TARGET GROUP: %p][LEADER: %p]\n", g_session->groups, g_session->groups->active_processes);
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				if (g_session->groups->active_processes->flags & (STOPPED | BACKGROUND))
				{
					if (PRINT_DEBUG)
					ft_dprintf(2, "[FG][KILL -SIGCONT \'%d\'][\'%p\']\n", g_session->groups->active_processes->pid, g_session->groups->active_processes);
					kill(g_session->groups->active_processes->pid, SIGCONT);
					if (PRINT_DEBUG)
					ft_dprintf(2, "[FG][UPDATE BACKGROUND]\n");
					update_background(&g_session->groups->active_processes, true);
					if (!(g_session->groups->active_processes->wstatus & STOPPED))
						ret = g_session->groups->active_processes->ret;
				}
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			if (!is_active_group(g_session->groups))
			{
				t_group*	fill = g_session->groups;
				g_session->groups->prev->next = g_session->groups->next;
				g_session->groups->next->prev = g_session->groups->prev;
				free(fill);
				fill = NULL;
				//group_remove(&g_session, &g_session->groups->prev, &g_session->groups->next);
			}
			else
				g_session->groups->active_processes = remember_leader;
			g_session->groups = remember;
			return (ret);
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
	return (ret);
}

int		ft_fg(t_exec* args, t_term* term)
{
	(void)term;
    t_process**		target;

	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: fg: %s: invalid option\n%s\n", args->av[1], "fg: usage: fg [job_spec]");
		return (CMD_BAD_USE);
	}
    if (session_empty() || g_session->groups->next == g_session->nil)
    {
        ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->ac == 1 ? "current" : args->av[1]);
        return (STD_ERROR);
    }
	
    target = g_session->groups->active_processes == g_session->groups->nil ? &g_session->groups->next->active_processes : &g_session->groups->active_processes;
    if (args->ac > 1)
    {
		// TO DO: if jobspec is pid has to resume is grou p or just the process ?
        if (!(target = jobspec_parser(args->ac, args->av, ignore_pid)))
		{
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
        }
    }
	if ((*target)->flags & RESTRICT_OP)
	{
		ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
	}
	//ft_dprintf(2, "target = %p\n", target);
	//ft_dprintf(2, "*target = %p\n", *target);
	//ft_dprintf(2, "active processes = %p\n", g_session->groups->active_processes == g_session->groups->nil ? g_session->groups->next->active_processes : g_session->groups->active_processes);

	print_job_args(STDERR_FILENO, *target);
	write(STDERR_FILENO, "\n", 1);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[FG] [session->groups before resume][%p]\n", g_session->groups);
    return (resume_group(*target));
}