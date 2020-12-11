/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   background.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:30:37 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:38:44 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>

#include <job_control/conditions.h>
#include <job_control/background.h>
#include <errors.h>
#include <libft.h>

/*
** Wait for the target process, store its return status and set flags.
*/

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

/*
** Takes a process as param, can compare its pid or wstatus to find the right
** object in the job control data structure and return a reference of it.
*/

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

/*
** Return the index of the target process in the job control data structure.
*/

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

/*
** Force each background group (running or stopped) to exit.
** Uses SIGHUP if the job wasn't disowned and SIGCONT for
** stopped groups. Then, it waits until the group has not
** terminated.
*/

void			background_force_exit(t_session *sess)
{
	t_group		*curr;
	t_process	*leader;

	curr = sess->groups;
	while (sess->groups != sess->nil)
	{
		leader = sess->groups->active_processes;
		while (sess->groups->active_processes != sess->groups->nil)
		{
			if (!(sess->groups->active_processes->flags & NO_HANGUP))
				kill(sess->groups->active_processes->pid, SIGHUP);
			kill(sess->groups->active_processes->pid, SIGCONT);
			if (!(sess->groups->active_processes->flags & (NO_HANGUP | EXITED)))
				while (waitpid(sess->groups->active_processes->pid, \
						&sess->groups->active_processes->wstatus, 0) <= 0)
					;
			sess->groups->active_processes = \
				sess->groups->active_processes->next;
		}
		sess->groups->active_processes = leader;
		sess->groups = sess->groups->next;
	}
	sess->groups = curr;
}

/*
** Return true if at least 1 background group is stopped.
*/

bool			is_background_stopped(void)
{
	t_group		*curr;

	curr = g_session.groups;
	while (g_session.groups != g_session.nil)
	{
		if (group_condition(g_session.groups, is_stopped))
		{
			g_session.groups = curr;
			return (true);
		}
		g_session.groups = g_session.groups->next;
	}
	g_session.groups = curr;
	return (false);
}
