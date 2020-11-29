/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:30:37 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 07:36:58 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>

#include <job_control.h>
#include <libft.h>

void			background_update(t_process **target)
{
	(*target)->flags &= ~(BACKGROUND | STOPPED);
	while (waitpid((*target)->pid, &(*target)->wstatus, WUNTRACED) <= 0)
		;
	if (WIFEXITED((*target)->wstatus))
		(*target)->ret = WEXITSTATUS((*target)->wstatus);
	else if (WIFSIGNALED((*target)->wstatus))
		(*target)->ret = SIGNAL_BASE + WTERMSIG((*target)->wstatus);
	else if (WIFSTOPPED((*target)->wstatus))
	{
		(*target)->ret = WSTOPSIG((*target)->wstatus);
		(*target)->flags |= STOPPED;
		history_session_update(group_get(*target));
	}
}

t_process		**background_find(t_process *target, const char *search_type,
		t_group *group)
{
	const char	*modes[2] = { "PID", "STA" };
	int			i;
	t_process	*remember;

	i = 0;
	remember = group->active_processes;
	while (i < 2 && ft_strncmp(modes[i], search_type, 3))
		i++;
	while (group->active_processes != group->nil)
	{
		if (!i && target->pid == group->active_processes->pid)
		{
			group->active_processes = remember;
			return (&group->active_processes);
		}
		else if (i && target->wstatus == group->active_processes->wstatus)
		{
			group->active_processes = remember;
			return (&group->active_processes);
		}
		group->active_processes = group->active_processes->next;
	}
	group->active_processes = remember;
	return (NULL);
}

size_t			background_index_get(t_group *nil, t_process *target)
{
	size_t		index;
	t_process	*fill;
	t_group		*groups;
	t_group		*remember;

	index = 0;
	groups = nil->prev;
	remember = groups;
	while (groups != nil && (++index))
	{
		fill = groups->nil->prev;
		while (fill != groups->nil)
		{
			if (fill == target)
			{
				nil->prev = remember;
				return (index);
			}
			fill = fill->prev;
		}
		groups = groups->prev;
	}
	nil->prev = remember;
	return (index);
}

void			background_force_exit(void)
{
	t_group		*curr;
	t_process	*leader;

	curr = g_session.groups;
	while (g_session.groups != g_session.nil)
	{
		leader = g_session.groups->active_processes;
		while (g_session.groups->active_processes != g_session.groups->nil)
		{
			if (!(g_session.groups->active_processes->flags & NO_HANGUP))
				kill(g_session.groups->active_processes->pid, SIGHUP);
			kill(g_session.groups->active_processes->pid, SIGCONT);
			if (!(g_session.groups->active_processes->flags & NO_HANGUP))
				while (waitpid(g_session.groups->active_processes->pid, \
						&g_session.groups->active_processes->wstatus, 0) <= 0)
					;
			g_session.groups->active_processes = \
				g_session.groups->active_processes->next;
		}
		g_session.groups->active_processes = leader;
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = curr;
}

bool			is_background_active(void)
{
	t_group		*curr;

	curr = g_session.groups;
	while (g_session.groups != g_session.nil)
	{
		if (group_condition(g_session.groups, is_active))
		{
			g_session.groups = curr;
			return (true);
		}
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = curr;
	return (false);
}
