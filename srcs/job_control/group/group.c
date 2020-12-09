/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:57:02 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:46:18 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/session.h>

/*
** Returns true if there's no processes in the target group given as parameter.
*/

bool			group_empty(t_group *group)
{
	return (group->active_processes && group->nil->next == group->nil \
		&& group->nil->prev == group->nil);
}

/*
** Link target between prev and next nodes.
*/

void			group_insert(t_group *prev, t_group *next, t_group *target)
{
	prev->next = target;
	next->prev = target;
	target->next = next;
	target->prev = prev;
}

/*
** Push a node in the group data structure using it as a stack.
*/

void			group_push_front(t_group *target)
{
	t_group		*fill;

	if (session_empty())
		group_insert(g_session.nil, g_session.nil, target);
	else
	{
		fill = g_session.nil->next;
		group_insert(g_session.nil, fill, target);
	}
	g_session.groups = g_session.nil->next;
}

/*
** Push a node in the group data structure using it as a queue.
*/

void			group_push_back(t_group *target)
{
	t_group		*fill;

	if (session_empty())
		group_insert(g_session.nil, g_session.nil, target);
	else
	{
		fill = g_session.nil->prev;
		group_insert(fill, g_session.nil->prev, target);
	}
	g_session.groups = g_session.nil->next;
}
