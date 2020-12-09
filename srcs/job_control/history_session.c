/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_session.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:11:48 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:32:27 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>
#include <job_control/conditions.h>
#include <libft.h>
#include <stdlib.h>

/*
** Push a node usign the history list as a stack and initialise
** it using the target group given as parameter.
*/

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

/*
** Pop a node in the history data structure using it as a stack.
*/

void				history_pop_front(void)
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

/*
** Remove a node containing the target group in the history data structure
** using it as a linked-list.
*/

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
	if (first)
		g_session.hist = first;
	else
		g_session.hist = next;
}

/*
** Destroys all the history node who has exited groups.
*/

void				history_session_purge_exited(void)
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
