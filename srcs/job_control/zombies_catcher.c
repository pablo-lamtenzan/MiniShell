/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies_catcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 01:45:31 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 08:06:20 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

void		handle_wstatus(t_group** group)
{
	if (WIFEXITED((*group)->active_processes->wstatus))
	{
		(*group)->active_processes->flags |= EXITED;
		(*group)->active_processes->ret = \
			WEXITSTATUS((*group)->active_processes->wstatus);
		deadzombie_push_back(deadzombie_new(&(*group)->active_processes));
	}
	else if (WIFSTOPPED((*group)->active_processes->wstatus))
	{
		g_session->zombies->exited = false;
		(*group)->active_processes->flags |= STOPPED;
		(*group)->active_processes->ret = SIGNAL_BASE + \
			WSTOPSIG((*group)->active_processes->wstatus);
	}
	else if (WIFSIGNALED((*group)->active_processes->wstatus))
		g_session->st = SIGNAL_BASE + \
			WTERMSIG((*group)->active_processes->wstatus);
}

void		catch_group(t_group** group)
{
	t_process*	leader;

	leader = (*group)->active_processes;
	while ((*group)->active_processes != (*group)->nil)
	{
		if ((*group)->active_processes->flags & BACKGROUND)
		{
			if (waitpid((*group)->active_processes->pid, \
				&(*group)->active_processes->wstatus, WNOHANG | WUNTRACED) > 0)
			{
				g_session->zombies->exited = true;
				(*group)->active_processes->flags &= ~BACKGROUND;
				handle_wstatus(group);
			}
		}
		(*group)->active_processes = (*group)->active_processes->next;
	}
	(*group)->active_processes = leader;
}

void				zombie_catcher(int signal)
{
	(void)signal;
	t_background*	first;

	// TO DO: change bool by bitwise
	if (!g_session || g_session->flags & RESTRICT_CATCH)
		return ;
	first = g_session->zombies;
	while (g_session->zombies)
	{
		if (g_session->zombies->background_group)
			catch_group(&g_session->zombies->background_group);
		g_session->zombies = g_session->zombies->next;
	}
	g_session->zombies = first;
}
