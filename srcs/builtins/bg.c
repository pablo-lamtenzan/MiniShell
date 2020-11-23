/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 23:11:42 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 14:28:11 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <job_control.h>
#include <signals.h>

void	resume_background_group(t_process* leader)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = g_session->groups;

	// skip itself
	ft_dprintf(2, "[BG][SKIPPED ITSELF: %p]\n", remember);
	g_session->groups = g_session->groups->next;
	//while (g_session->groups->active_processes->flags & BACKGROUND)
	//	g_session->groups = g_session->groups->next;
	while (g_session->groups != g_session->nil)
	{
		ft_dprintf(2, "test:::::: %p [%d] --- %p [%d] \n", g_session->groups->active_processes, g_session->groups->active_processes->pid, leader, leader->pid);
		if (g_session->groups->active_processes->pid == leader->pid)
		{
			ft_dprintf(2, "[BG][TARGET GROUP: %p][LEADER: %p]\n", g_session->groups, g_session->groups->active_processes);
			remember_leader = g_session->groups->active_processes;
			while (g_session->groups->active_processes != g_session->groups->nil)
			{
				if (g_session->groups->active_processes->flags & STOPPED)
				{
					ft_dprintf(2, "[BG][KILL -SIGCONT \'%d\'][\'%p\']\n", g_session->groups->active_processes->pid, g_session->groups->active_processes);
					g_session->groups->active_processes->flags &= ~STOPPED;
					g_session->groups->active_processes->flags |= BACKGROUND;
					kill(g_session->groups->active_processes->pid, SIGCONT);
					ft_dprintf(2, "[BG][TARGET FLAGS: %d][\'%p\']\n", g_session->groups->active_processes->flags, g_session->groups->active_processes);
					//update_background(g_session, &g_session->groups->active_processes, true);
				}
				g_session->groups->active_processes = g_session->groups->active_processes->next;
			}
			/*
			if (!is_active_group(g_session->groups))
			{
				t_group*	fill = g_session->groups;
				g_session->groups->prev->next = g_session->groups->next;
				g_session->groups->next->prev = g_session->groups->prev;
				free(fill);
				fill = NULL;
				//group_remove(&g_session, &g_session->groups->prev, &g_session->groups->next);
			}
			else*/
				g_session->groups->active_processes = remember_leader;
			g_session->groups = remember;
			return ;
		}
		g_session->groups = g_session->groups->next;
	}
	g_session->groups = remember;
}

int		ft_bg(t_exec* args, t_term* term)
{
    t_process**		target;
	(void)term;

	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: %s: invalid option\n%s\n", args->av[1], "bg: usage: bg [job_spec]");
		return (CMD_BAD_USE);
	}
    if (session_empty() || g_session->groups->next == g_session->nil)
    {
        ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->ac == 1 ? "current" : args->av[1]);
        return (STD_ERROR);
    }
	t_group*	remember;
	remember = g_session->groups;
	while (g_session->groups != g_session->nil->prev)
	{
    	target = g_session->groups->active_processes == g_session->groups->nil ? &g_session->groups->next->active_processes : &g_session->groups->active_processes;
		if ((*target)->flags & BACKGROUND)
			g_session->groups = g_session->groups->next;
		else
			break ;
	}
	g_session->groups = remember;
    if (args->ac > 1)
    {
		// TO DO: if jobspec is pid has to resume is grou p or just the process ?
        if (!(target = jobspec_parser(args->ac, args->av, NULL)))
		{
            ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
        }
    }
	if ((*target)->flags & RESTRICT_OP)
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
	}
	if ((*target)->flags & BACKGROUND)
	{
		ft_dprintf(STDERR_FILENO, "minish: job %lu already in background\n", get_background_index(g_session->nil, *target));
		return (SUCCESS);
	}
	//ft_dprintf(2, "target = %p\n", target);
	//ft_dprintf(2, "*target = %p\n", *target);
	//ft_dprintf(2, "active processes = %p\n", term->session->groups->active_processes == term->session->groups->nil ? term->session->groups->next->active_processes : term->session->groups->active_processes);

	// 
	print_index_args(*target);
	write(STDERR_FILENO, " &\n", 3);
	ft_dprintf(2, "[BG] [session->groups before resume][%p]\n", g_session->groups);
	// termary for skip itself, leader must be next->active_processes

	resume_background_group(*target);
	ft_dprintf(2, "[BG] [session->groups after resume][%p]\n", g_session->groups);
	ft_dprintf(2, "[BG]ACTIVE PROCESSES AT THE END: \'%p\'\n", g_session->groups->active_processes == g_session->groups->nil ? g_session->groups->next->active_processes : g_session->groups->active_processes);

    //update_background(term->session, target);
	// I haven't to wait but i need the status to know when it finishs (TO THINK ABOUT)
	// Can use SIGHILD to know when it ends
	// https://softwareengineering.stackexchange.com/questions/162940/how-do-i-implement-the-bg-and-fg-commands-functionaliity-in-my-custom-unix-s
	// https://stackoverflow.com/questions/7171722/how-can-i-handle-sigchld (doesn t respect the subject but very interesting)
	// https://docs.oracle.com/cd/E19455-01/806-4750/signals-7/index.html (this follows the subject)

	// can use command ps ax for get the pid table, then i can split it and rm the i % 2 != 0 of the split
    return (SUCCESS);
}
