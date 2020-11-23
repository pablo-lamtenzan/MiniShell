/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/18 23:11:42 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 05:20:30 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>
#include <signals.h>

void	resume_background_group(t_session* session, t_process* leader)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = session->groups;

	// skip itself
	ft_dprintf(2, "[BG][SKIPPED ITSELF: %p]\n", remember);
	session->groups = session->groups->next;
	//while (session->groups->active_processes->flags & BACKGROUND)
	//	session->groups = session->groups->next;
	while (session->groups != session->nil)
	{
		ft_dprintf(2, "test:::::: %p [%d] --- %p [%d] \n", session->groups->active_processes, session->groups->active_processes->pid, leader, leader->pid);
		if (session->groups->active_processes->pid == leader->pid)
		{
			ft_dprintf(2, "[BG][TARGET GROUP: %p][LEADER: %p]\n", session->groups, session->groups->active_processes);
			remember_leader = session->groups->active_processes;
			while (session->groups->active_processes != session->groups->nil)
			{
				ft_dprintf(2, "[BG][KILL -SIGCONT \'%d\'][\'%p\']\n", session->groups->active_processes->pid, session->groups->active_processes);
				session->groups->active_processes->flags &= ~STOPPED;
				session->groups->active_processes->flags |= BACKGROUND;
				kill(session->groups->active_processes->pid, SIGCONT);
				ft_dprintf(2, "[BG][TARGET FLAGS: %d][\'%p\']\n", session->groups->active_processes->flags, session->groups->active_processes);
				//update_background(session, &session->groups->active_processes, true);
				session->groups->active_processes = session->groups->active_processes->next;
			}
			/*
			if (!is_active_group(session->groups))
			{
				t_group*	fill = session->groups;
				session->groups->prev->next = session->groups->next;
				session->groups->next->prev = session->groups->prev;
				free(fill);
				fill = NULL;
				//group_remove(&session, &session->groups->prev, &session->groups->next);
			}
			else*/
				session->groups->active_processes = remember_leader;
			session->groups = remember;
			return ;
		}
		session->groups = session->groups->next;
	}
	session->groups = remember;
}

int		ft_bg(t_exec* args, t_term* term)
{
    t_process**		target;

	if (args->av[1] && args->av[1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "minish: bg: %s: invalid option\n%s\n", args->av[1], "bg: usage: bg [job_spec]");
		return (CMD_BAD_USE);
	}
    if (session_empty(term->session) || term->session->groups->next == term->session->nil)
    {
        ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->ac == 1 ? "current" : args->av[1]);
        return (STD_ERROR);
    }
	t_group*	remember;
	remember = term->session->groups;
	while (term->session->groups != term->session->nil)
	{
    	target = term->session->groups->active_processes == term->session->groups->nil ? &term->session->groups->next->active_processes : &term->session->groups->active_processes;
		if ((*target)->flags & BACKGROUND)
			term->session->groups = term->session->groups->next;
		else
			break ;
	}
	term->session->groups = remember;
    if (args->ac > 1)
    {
		// TO DO: if jobspec is pid has to resume is grou p or just the process ?
        if (!(target = jobspec_parser(term->session, args->ac, args->av, NULL)))
		{
            ft_dprintf(STDERR_FILENO, "minish: bg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
        }
    }
	//ft_dprintf(2, "target = %p\n", target);
	//ft_dprintf(2, "*target = %p\n", *target);
	//ft_dprintf(2, "active processes = %p\n", term->session->groups->active_processes == term->session->groups->nil ? term->session->groups->next->active_processes : term->session->groups->active_processes);

	// 
	print_index_args(term->session, *target);
	write(STDERR_FILENO, " &\n", 3);
	ft_dprintf(2, "[BG] [session->groups before resume][%p]\n", term->session->groups);
	// termary for skip itself, leader must be next->active_processes

	resume_background_group(term->session, *target);
	ft_dprintf(2, "[BG] [session->groups after resume][%p]\n", term->session->groups);
	ft_dprintf(2, "[BG]ACTIVE PROCESSES AT THE END: \'%p\'\n", term->session->groups->active_processes == term->session->groups->nil ? term->session->groups->next->active_processes : term->session->groups->active_processes);

    //update_background(term->session, target);
	// I haven't to wait but i need the status to know when it finishs (TO THINK ABOUT)
	// Can use SIGHILD to know when it ends
	// https://softwareengineering.stackexchange.com/questions/162940/how-do-i-implement-the-bg-and-fg-commands-functionaliity-in-my-custom-unix-s
	// https://stackoverflow.com/questions/7171722/how-can-i-handle-sigchld (doesn t respect the subject but very interesting)
	// https://docs.oracle.com/cd/E19455-01/806-4750/signals-7/index.html (this follows the subject)

	// can use command ps ax for get the pid table, then i can split it and rm the i % 2 != 0 of the split
    return (SUCCESS);
}
