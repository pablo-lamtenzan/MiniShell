/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_process_destructors.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:54:07 by pablo             #+#    #+#             */
/*   Updated: 2020/11/27 00:55:14 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

// here process remove

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