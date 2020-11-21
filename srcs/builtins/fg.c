/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 09:32:38 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 18:44:16 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>

#include <execution.h>
#include <process.h>


void	resume_group(t_session* session, t_process* leader)
{
	t_group*	remember;
	t_process*	remember_leader;

	remember = session->groups;

	// skip itself
	ft_dprintf(2, "[FG][SKIPPED ITSELF: %p]\n", remember);
	session->groups = session->groups->next;
	while (session->groups != session->nil)
	{
		ft_dprintf(2, "test:::::: %p [%d] --- %p [%d] \n", session->groups->active_processes, session->groups->active_processes->pid, leader, leader->pid);
		if (session->groups->active_processes->pid == leader->pid)
		{
			ft_dprintf(2, "[FG][TARGET GROUP: %p][LEADER: %p]\n", session->groups, session->groups->active_processes);
			remember_leader = session->groups->active_processes;
			while (session->groups->active_processes != session->groups->nil)
			{
				ft_dprintf(2, "[FG][KILL -SIGCONT \'%d\'][\'%p\']\n", session->groups->active_processes->pid, session->groups->active_processes);
				kill(session->groups->active_processes->pid, SIGCONT);
				ft_dprintf(2, "[FG][UPDATE BACKGROUND]\n");
				update_background(session, &session->groups->active_processes);
				session->groups->active_processes = session->groups->active_processes->next;
			}
			if (!is_active_group(session->groups))
			{
				t_group*	fill = session->groups;
				session->groups->prev->next = session->groups->next;
				session->groups->next->prev = session->groups->prev;
				free(fill);
				fill = NULL;
				//group_remove(&session, &session->groups->prev, &session->groups->next);
			}
			else
				session->groups->active_processes = remember_leader;
			session->groups = remember;
			return ;
		}
		session->groups = session->groups->next;
	}
	session->groups = remember;
}

int		ft_fg(t_exec* args, t_term* term)
{
    t_process**		target;

    if (session_empty(term->session) || term->session->groups->next == term->session->nil)
    {
        ft_dprintf(STD_ERROR, "minish: fg: current: no such job\n");
        return (STD_ERROR);
    }

    target = term->session->groups->active_processes == term->session->groups->nil ? &term->session->groups->next->active_processes : &term->session->groups->active_processes;
    if (args->ac > 1)
    {
		// TO DO: if jobspec is pid has to resume is grou p or just the process ?
        if (!(target = jobspec_parser(term->session, args->ac, args->av, NULL)))
		{
            ft_dprintf(STDERR_FILENO, "minish: fg: %s: no such job\n", args->av[1]);
            return (STD_ERROR);
        }
    }
	//ft_dprintf(2, "target = %p\n", target);
	//ft_dprintf(2, "*target = %p\n", *target);
	//ft_dprintf(2, "active processes = %p\n", term->session->groups->active_processes == term->session->groups->nil ? term->session->groups->next->active_processes : term->session->groups->active_processes);

	
	ft_dprintf(2, "[FG] [session->groups before resume][%p]\n", term->session->groups);
	// termary for skip itself, leader must be next->active_processes
	resume_group(term->session, *target);
	ft_dprintf(2, "[FG] [session->groups after resume][%p]\n", term->session->groups);
	ft_dprintf(2, "[FG]ACTIVE PROCESSES AT THE END: \'%p\'\n", term->session->groups->active_processes == term->session->groups->nil ? term->session->groups->next->active_processes : term->session->groups->active_processes);
    return (SUCCESS);
}