/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:44:40 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 01:50:38 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

t_process		**get_process_by_index(t_group* groups, size_t index)
{
	if (!index)
		return (NULL);
	ft_dprintf(2, "[INDEX][index: %lu]\n", index);
	while (index && groups->next != g_session->nil)
	{
		index--;
		groups = groups->next;
	}
	return (index ? NULL : &groups->nil->next);
}

t_process		**get_process_by_pid(t_group* groups, pid_t pid)
{
	t_process fill;

	fill.pid = pid;
	while (groups != g_session->nil)
	{
		if (background_find(&fill, "PID", groups))
			return (&groups->active_processes);
		groups = groups->next;
	}
	return (NULL);
}

// get process by name (unfinished) in another .c

t_process		**get_process_by_history(t_group* groups, size_t index)
{
	t_history*	hist_addr;

	hist_addr = NULL;
	if (!g_session->hist || !index)
		return (NULL);
	else if (!g_session->hist->next || index == 1)
		hist_addr = g_session->hist;
	else if (index == 2)
		hist_addr = g_session->hist->next;
	return (get_process_by_pid(groups, hist_addr->group->nil->next->pid));
}

t_process		**process_search(char*const* av)
{
	if (is_jobspec(av[1]))
	{
		if (is_string_digit(&av[1][1]))
			return (get_process_by_index(g_session->groups, ft_atoi(&av[1][1])));
		else if (is_history_process(av[1]))
			return (get_process_by_history(g_session->groups, get_history_index(&av[1][1])));
		else
			return (get_process_by_name(g_session->groups, &av[1][1]));
	}
	else if (is_string_digit(av[1]))
		return (get_process_by_pid(g_session->groups, ft_atoi(av[1])));
	else
		return (NULL);
}

t_process**		jobspec_parser(int ac, char*const* av, bool (*fill)(int ac, char*const* av))
{
	if (fill && !fill(ac, av))
		return (NULL);
	return (process_search(av));
}