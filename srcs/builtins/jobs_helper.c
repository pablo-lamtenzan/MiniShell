/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 01:53:32 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 08:37:59 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <signals_print.h>

void			print_process(int fd, t_process *target, int flags)
{
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

void			print_group(int fd, t_process *leader, int flags,
		t_group *itself)
{
	t_group		*remember;
	t_process	*remember_leader;

	remember = g_session.groups;
	if (g_session.groups == itself)
		g_session.groups = g_session.groups->next;
	while (g_session.groups != g_session.nil)
	{
		if (g_session.groups->nil->next->pid == leader->pid)
		{
			remember_leader = g_session.groups->active_processes;
			while (g_session.groups->active_processes \
				!= g_session.groups->nil)
			{
				print_process(fd, g_session.groups->active_processes, flags);
				g_session.groups->active_processes = \
					g_session.groups->active_processes->next;
			}
			g_session.groups->active_processes = remember_leader;
			break ;
		}
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = remember;
}

// TO DO: test to exclude all signals exept SIGCONT here
bool			is_not_running(t_process* target)
{
	return (WIFEXITED(target->wstatus) || WIFSTOPPED(target->wstatus));
}

bool			is_not_stopped(t_process* target)
{
	return (WIFEXITED(target->wstatus) || !WIFSTOPPED(target->wstatus));
}
