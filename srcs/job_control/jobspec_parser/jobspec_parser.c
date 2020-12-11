/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:44:40 by pablo             #+#    #+#             */
/*   Updated: 2020/12/10 20:05:43 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/background.h>
#include <job_control/session.h>
#include <job_control/jobspec_parser.h>
#include <job_control/utils.h>
#include <libft.h>

/*
** Performs a search in the job control data structure returning a reference
** to the group who contains the index given as parameter or else NULL.
*/

t_process		**get_process_by_index(t_group *groups, size_t index)
{
	if (!index)
		return (NULL);
	while (index && groups->next != g_session.nil)
	{
		index--;
		groups = groups->next;
	}
	return (index ? NULL : &groups->nil->next);
}

/*
** Performs a search in the job control data structure returning a reference
** to the group who contains the pid given as parameter or else NULL.
*/

t_process		**get_process_by_pid(t_group *groups, pid_t pid)
{
	t_process fill;

	fill.pid = pid;
	while (groups != g_session.nil)
	{
		if (background_find(&fill, "PID", groups))
			return (&groups->active_processes);
		groups = groups->next;
	}
	return (NULL);
}

/*
** Performs a search in the history returning a reference
** to the group who contains the pid given as parameter or else NULL.
*/

t_process		**get_process_by_history(t_group *groups, size_t index)
{
	t_history	*hist_addr;

	hist_addr = NULL;
	if (!g_session.hist || !index)
		return (NULL);
	else if (!g_session.hist->next || index == 1)
		hist_addr = g_session.hist;
	else if (index == 2)
		hist_addr = g_session.hist->next;
	return (!hist_addr || !hist_addr->group ? NULL \
	: get_process_by_pid(groups, hist_addr->group->nil->next->pid));
}

/*
** Perform a search supporting the POSIX job specification patterns.
*/

t_process		**process_search(char*const *av)
{
	if (is_jobspec(av[1]))
	{
		if (is_string_digit(&av[1][1]))
			return (get_process_by_index(g_session.groups, ft_atoi(&av[1][1])));
		else if (is_history_process(av[1]))
			return (get_process_by_history(g_session.groups, \
				get_history_index(&av[1][1])));
		else
			return (get_process_by_name(g_session.groups, &av[1][1]));
	}
	else if (is_string_digit(av[1]))
		return (get_process_by_pid(g_session.groups, ft_atoi(av[1])));
	else
		return (NULL);
}

/*
** Call a condition and proceed to search if the condition return true.
** Return a reference to the process found.
*/

t_process		**jobspec_parser(int ac, char*const *av,
		bool (*fill)(int ac, char*const *av))
{
	if (fill && !fill(ac, av))
		return (NULL);
	return (process_search(av));
}
