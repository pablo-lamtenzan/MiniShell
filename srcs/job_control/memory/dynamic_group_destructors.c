/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_group_destructors.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:41:09 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:22:05 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/destructors.h>
#include <job_control/group.h>
#include <job_control/session.h>
#include <libft.h>

#include <stdlib.h>

/*
** Remove the target node in the group data structure
** using it as a doubly-linked-list.
*/

void			group_remove(t_group **target)
{
	t_group		**next;
	t_group		**prev;

	next = &(*target)->next;
	prev = &(*target)->prev;
	(*next)->prev = *prev;
	(*prev)->next = *next;
	delete_group_input(target);
	delete_processes(target);
	free((*target)->nil);
	free(*target);
	*target = NULL;
}

/*
** Pop a node in the group data structure using it as a stack.
*/

void			group_pop_front(void)
{
	t_group		*fill;

	if (session_empty())
		return ;
	fill = g_session.nil->next->next;
	group_remove(&g_session.groups);
	g_session.nil->next = fill;
	g_session.groups = g_session.nil->next;
}

/*
** Pop a node in the group data structure using it as a queue.
*/

void			group_pop_back(void)
{
	t_group		*fill;

	if (session_empty())
		return ;
	fill = g_session.nil->prev->prev;
	group_remove(&g_session.groups);
	g_session.nil->prev = fill;
	g_session.groups = g_session.nil->next;
}
