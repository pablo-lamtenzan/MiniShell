/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:07:32 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:36:09 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

void			process_insert(t_process *prev, t_process *next,
		t_process *target)
{
	prev->next = target;
	next->prev = target;
	target->next = next;
	target->prev = prev;
}

void			process_push_front(t_group **group, t_process *target)
{
	t_process	*fill;

	if (group_empty(*group))
		process_insert((*group)->nil, (*group)->nil, target);
	else
	{
		fill = (*group)->nil->next;
		process_insert((*group)->nil, fill, target);
	}
	(*group)->active_processes = (*group)->nil->next;
}

void			process_push_back(t_group **group, t_process *target)
{
	t_process	*fill;

	if (group_empty(*group))
		process_insert((*group)->nil, (*group)->nil, target);
	else
	{
		fill = (*group)->nil->prev;
		process_insert(fill, (*group)->nil, target);
	}
	(*group)->active_processes = (*group)->nil->next;
}
