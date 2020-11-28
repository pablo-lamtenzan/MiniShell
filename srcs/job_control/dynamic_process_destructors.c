/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_process_destructors.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:54:07 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 03:53:02 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

/*
replaces remove_process
*/
// HAVE TO TEST IT AND THEN CHANGE ITS NAME AND REPLACE PROCESS REMOVE

void			remove_process(t_process** target)
{
	t_process**	next;
	t_process**	prev;

	next = &(*target)->next;
	prev = &(*target)->prev;
	(*next)->prev = *prev;
	(*prev)->next = *next;
	free(*target);
	*target = NULL;
}


void		process_remove(t_process** prev, t_process** next)
{
	t_process* fill;
	t_process* fill2;

	fill = (*prev)->next;
	if (*prev == *next)
	{
		free(fill);
		// + data
		// incomplete need reset the nil
		return ; 
	}
	while (fill != *next)
	{
		fill2 = fill;
		fill = fill->next;
		free(fill2); // delete data too
		fill2 = NULL;
	}
	(*prev)->next = *next;
	(*next)->prev = *prev;
}


void		process_pop_back(t_group** group)
{
	t_process*	fill;

	if (group_empty(*group))
		return ;
	fill = (*group)->nil->prev->prev;
	process_remove(&(*group)->nil->prev->prev, &(*group)->nil->prev);
	(*group)->nil->prev = fill;
	(*group)->active_processes = (*group)->nil->next;
}

void		process_pop_font(t_group** group)
{
	t_process* fill;

	if (group_empty(*group))
		return ;
	fill = (*group)->nil->next->next;
	process_remove(&(*group)->nil, &(*group)->nil->next->next);
	(*group)->nil->next = fill;
	(*group)->active_processes = (*group)->nil->next;
}