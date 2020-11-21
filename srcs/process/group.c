/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:26:31 by pablo             #+#    #+#             */
/*   Updated: 2020/11/21 01:34:40 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <process.h>
#include <libft.h>
#include <stdlib.h>

/*
** Allocates and init a new group
*/
t_group			*group_new()
{
	t_group*	group;

	if (!(group = ft_calloc(1, sizeof(t_group)))
			|| !(group->nil = ft_calloc(1, sizeof(t_process))))
		return (NULL);
	group->nil->next = group->nil;
	group->nil->prev = group->nil;
	group->active_processes = group->nil;
	ft_dprintf(2, "[GROUP NEW][\'%p\'][nil=\'%p\']\n", group, group->nil);
	return (group);
}

/*
** Returns true if the session is empty
*/
bool			session_empty(t_session *target)
{
	return (target->nil->next == target->nil && target->nil->prev == target->nil);
}

/*
** Inserts target between prev and next
*/
void			group_insert(t_group* prev, t_group* next, t_group* target)
{
	prev->next = target;
	next->prev = target;
	target->next = next;
	target->prev = target;
}

/*
** Remove all group nodes between prev and next
*/
void			group_remove(t_session** session, t_group** prev, t_group** next)
{
	//t_group*	fill;
	//t_group*	fill2;

	//fill = (*prev)->next;
	ft_dprintf(2, "NEXTtttttttttt: %p\n", *next);
	ft_dprintf(2, "Prevvvvvvvvvvvvvv: %p\n", *prev);
	if (*prev == *next)
	{
		ft_dprintf(2, "[GROUP REMOVE][REMOVE:\'%p\']\n", (*prev)->next);
		free((*prev)->next->nil);
		free((*prev)->next);
		(*prev)->next = NULL;
		(*session)->nil->next = (*session)->nil;
		(*session)->nil->prev = (*session)->nil;
	}
	else
	{
		ft_dprintf(2, "[GROUP REMOVE][AT LEAST 1 PROCESS STTOPEED OR IN BACKGROUND!]\n");
		//while (fill != *next)
		//{
			ft_dprintf(2, "[GROUP REMOVE][REMOVE:\'%p\'] nil= %p prev= %p \n",(*prev)->next, (*prev)->next->nil, *prev);
			free((*prev)->next->nil);
			free((*prev)->next); // remove it process too
			ft_dprintf(2, " %p ----- %p \n", (*prev), NULL/**next*/);
			(*prev)->next = NULL;
		//}
		//(*prev)->next = *next;
		//(*next)->prev = *prev;
	}
}

/*
** Adds a new group after the head
*/
void			group_push_front(t_session** session, t_group* target)
{
	t_group*	fill;

	if (session_empty(*session))
		group_insert((*session)->nil, (*session)->nil, target);
	else
	{
		fill = (*session)->nil->next;
		group_insert((*session)->nil, fill, target);
	}
	(*session)->groups = (*session)->nil->next;
	ft_dprintf(2, "[GROUP PUSH FRONT][CURR GROUP NOW IS: \'%p\']\n", (*session)->groups);
}

/*
** Adds a group before the tail
*/
void			group_push_back(t_session** session, t_group* target)
{
	t_group*	fill;

	if (session_empty(*session))
		group_insert((*session)->nil, (*session)->nil, target);
	else
	{
		fill = (*session)->nil->prev;
		group_insert(fill, (*session)->nil->prev, target);
	}
	(*session)->groups = (*session)->nil->next;
}

/*
** Removes a group after the head
*/
void			group_pop_front(t_session** session)
{
	t_group*	fill;
	
	if (session_empty(*session))
		return ;
	fill = (*session)->nil->next->next;
	ft_dprintf(2, "<<>>>>>>%p\n", (*session)->nil->next);
	group_remove(session, &(*session)->nil, &(*session)->nil->next->next);
	(*session)->nil->next = fill;
	(*session)->groups = (*session)->nil->next;
	ft_dprintf(2, "[GROUP POP FRONT][CURR GROUP IS NOW: \'%p\']\n", (*session)->groups);
}

/*
** Removes a group before the tail
*/
void			group_pop_back(t_session** session)
{
	t_group*	fill;

	if (session_empty(*session))
		return ;
	fill = (*session)->nil->prev->prev;
	group_remove(session, &(*session)->nil->prev->prev, &(*session)->nil->prev);
	(*session)->nil->prev = fill;
	(*session)->groups = (*session)->nil->next;
}
