/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_process_destructors.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:54:07 by pablo             #+#    #+#             */
/*   Updated: 2020/12/09 23:22:47 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control/destructors.h>
#include <job_control/group.h>
#include <job_control/session.h>
#include <libft.h>

#include <stdlib.h>

/*
** Remove the target node in the process data structure
** using it as a doubly-linked-list.
*/

void			process_remove(t_process **target)
{
	t_process	**next;
	t_process	**prev;

	next = &(*target)->next;
	prev = &(*target)->prev;
	(*next)->prev = *prev;
	(*prev)->next = *next;
	free(*target);
	*target = NULL;
}

/*
** Pop a node in the process data structure using it as a queue.
*/

void			process_pop_back(t_group **group)
{
	t_process	*fill;

	if (group_empty(*group))
		return ;
	fill = (*group)->nil->prev->prev;
	process_remove(&(*group)->nil->prev);
	(*group)->nil->prev = fill;
	(*group)->active_processes = (*group)->nil->next;
}

/*
** Pop a node in the process data structure using it as a stack.
*/

void			process_pop_font(t_group **group)
{
	t_process	*fill;

	if (group_empty(*group))
		return ;
	fill = (*group)->nil->next->next;
	process_remove(&(*group)->nil->next);
	(*group)->nil->next = fill;
	(*group)->active_processes = (*group)->nil->next;
}
