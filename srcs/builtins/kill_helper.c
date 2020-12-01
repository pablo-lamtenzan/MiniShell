/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 02:42:01 by pablo             #+#    #+#             */
/*   Updated: 2020/12/01 09:58:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>
#include <signal.h>

const char			*get_signal(const char *key, int *res)
{
	int				i;

	i = 0;
	if ((*key == 'l' || *key == 'L') && !*(key + 1))
	{
		*res = 256;
		return ("");
	}
	while (i < 31 && ft_strncmp(g_signals[i], key, ft_strlen(g_signals[i]))
			&& ft_strncmp(&g_signals[i][3], key, ft_strlen(g_signals[i]) - 3)
			&& ft_strncmp(g_cvalues[i], key, ft_strlen(g_cvalues[i]) + 1))
		i++;
	if (i == 31 && !(*res = 0))
		return (NULL);
	*res = g_values[i];
	return (*res < 0 ? NULL : g_signals[i]);
}

void				print_all_signals(void)
{
	int				i;
	int				tmp;

	i = -1;
	while (++i < 31)
	{
		ft_dprintf(STDERR_FILENO, "%2d) %-9s ", i + 1, \
			get_signal(g_cvalues[i], &tmp));
		if ((i + 1) % 5 == 0)
			write(STDERR_FILENO, "\n", 1);
	}
	write(STDERR_FILENO, "\n", 1);
}

void				kill_core(int signal)
{
	kill(g_session.groups->active_processes->pid, signal);
	if (!(signal >= SIGSTOP && signal <= SIGTTOU) && signal != SIGCONT)
	{
		g_session.groups->active_processes->flags |= (SIGNALED | KILLED);
		kill(g_session.groups->active_processes->pid, SIGCONT);
	}
	background_update(&g_session.groups->active_processes);
	history_session_remove_node(g_session.groups);
}

int					handle_current(t_process ***target,
	const char *jobspec)
{
	t_group			*remember;

	remember = g_session.groups;
	if (!ft_strncmp(jobspec, "%", 2) || !ft_strncmp(jobspec, "%+", 3) \
		|| !ft_strncmp(jobspec, "%%", 3) \
		|| (g_session.hist && !g_session.hist->next \
		&& !ft_strncmp(jobspec, "%-", 3)))
	{
		while (g_session.groups != g_session.nil->prev)
		{
			*target = g_session.groups->next != g_session.nil ? \
				&g_session.groups->next->active_processes : \
				&g_session.groups->active_processes;
			if ((**target)->flags & (KILLED | SIGNALED))
				g_session.groups = g_session.groups->next;
			else
				break ;
		}
		g_session.groups = remember;
		return (true);
	}
	return (false);
}

void				kill_group(t_process *leader, int signal,
		t_group *itself)
{
	t_group			*remember;
	t_process		*remember_leader;

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
				kill_core(signal);
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
