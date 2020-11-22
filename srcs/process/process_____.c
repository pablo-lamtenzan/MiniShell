/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_____.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:05:59 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 23:40:55 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <process.h>
#include <libft.h>
#include <stdlib.h>

/*
** Alloocates and init a new process
*/
t_process*		process_new(pid_t pid, int wstatus, char*const* data)
{
	t_process*	process;

	if (!(process = ft_calloc(1, sizeof(t_process))))
		return (NULL);
	*process = (t_process){.pid=pid, .wstatus=wstatus, .data=data};
	ft_dprintf(2, "[NEW PROCESS][\'%p\']\n", process);
	return (process);
}

/*
** Returns if the group is empty
*/
bool		group_empty(t_group* group)
{
	return (group->nil->next == group->nil && group->nil->prev == group->nil);
}

/*
** Inserts target between prev and next
*/
void		process_insert(t_process* prev, t_process* next, t_process* target)
{
	prev->next = target;
	next->prev = target;
	target->next = next;
	target->prev = prev;
}

/*
** Remove all process nodes between prev and next
*/
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

/*
** Adds a new process after the head
*/
void		process_push_front(t_group** group, t_process* target)
{
	t_process* fill;

	if (group_empty(*group))
		process_insert((*group)->nil, (*group)->nil, target);
	else
	{
		fill = (*group)->nil->next;
		process_insert((*group)->nil, fill, target);
	}
	(*group)->active_processes = (*group)->nil->next;
}

/*
** Adds a process before the tail
*/
void		process_push_back(t_group** group, t_process* target)
{
	t_process* fill;

	if (group_empty(*group))
		process_insert((*group)->nil, (*group)->nil, target);
	else
	{
		fill = (*group)->nil->prev;
		process_insert(fill, (*group)->nil, target);
	}
	ft_dprintf(2, "[PROCESS PUSH FRONT][LAST PROCESS NOW IS: \'%p\']\n", (*group)->nil->prev);
	(*group)->active_processes = (*group)->nil->next;
	ft_dprintf(2, "[PROCESS PUSH BACK][FIRST PROCESS NOW IS: \'%p\']\n", (*group)->active_processes);
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

/*
** Removes a process before the tail
*/
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