/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_process_destructors.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:54:07 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:35:30 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

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
