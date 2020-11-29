/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_session.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:11:48 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 03:45:26 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

bool				history_session_update(t_group *update)
{
	t_history		*next;
	t_history		*hist;

	if (!update || !(hist = ft_calloc(1, sizeof(t_history))))
		return (false);
	*hist = (t_history){.group=update};
	next = g_session.hist;
	g_session.hist = hist;
	hist->next = next;
	return (true);
}

void				history_pop_front()
{
	t_history		*fill;

	if (g_session.hist)
	{
		fill = g_session.hist->next;
		free(g_session.hist);
		g_session.hist = fill;
	}
}

static void			remove_hist_node(t_history **prev, t_history **next, 
	t_history **first)
{
	if (*prev)
		(*prev)->next = g_session.hist->next;
	if (*first == g_session.hist)
		*first = NULL;
	if (*next == g_session.hist)
		*next = (*next)->next;
	free(g_session.hist);
	g_session.hist = NULL;
}

void				history_session_remove_node(t_group *target)
{
	t_history		*prev;
	t_history		*next;
	t_history		*first;

	first = g_session.hist;
	prev = NULL;
	next = NULL;
	if (g_session.hist)
		next = g_session.hist->next;
	while (g_session.hist)
	{
		if (target->nil->next->pid == g_session.hist->group->nil->next->pid)
		{
			remove_hist_node(&prev, &next, &first);
			break ;
		}
		prev = g_session.hist;
		g_session.hist = g_session.hist->next;
	}
	if (first && first->group && first->group->nil->next->pid \
			== target->nil->next->pid)
		g_session.hist = first->next;
	else
		g_session.hist = next;
}

void				history_session_purge_exited()
{
	t_history		*first;
	t_history		*next;

	first = g_session.hist;

	while (g_session.hist)
	{
		next = g_session.hist->next;
		if (group_condition(g_session.hist->group, is_exited))
		{
			if (first == g_session.hist)
				first = first->next;
			free(g_session.hist);
		}
		g_session.hist = next;
	}
	g_session.hist = first;
}

