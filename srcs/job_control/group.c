/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   group.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 18:26:31 by pablo             #+#    #+#             */
/*   Updated: 2020/11/25 00:41:10 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
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
	if (PRINT_DEBUG)
		ft_dprintf(2, "[GROUP NEW][\'%p\'][nil=\'%p\']\n", group, group->nil);
	return (group);
}

/*
** Returns true if the session is empty
*/
bool			session_empty()
{
	return (g_session->nil->next == g_session->nil && g_session->nil->prev == g_session->nil);
}

/*
** Inserts target between prev and next
*/
void			group_insert(t_group* prev, t_group* next, t_group* target)
{
	prev->next = target;
	next->prev = target;
	target->next = next;
	target->prev = prev;
}

/*
*/
void			group_remove_v2(t_group** target)
{
	t_group**	next;
	t_group**	prev;

	next = &(*target)->next;
	prev = &(*target)->prev;
	(*next)->prev = *prev;
	(*prev)->next = *next;
	if (PRINT_DEBUG)
		ft_dprintf(2, "REMOVE GROUP: %p\n" ,*target);
	free(*target);
	//*target = NULL;
}
void			group_remove(t_group** prev, t_group** next)
{
	//t_group*	fill;
	//t_group*	fill2;

	//fill = (*prev)->next;
	//ft_dprintf(2, "NEXTtttttttttt: %p\n", *next);
	//ft_dprintf(2, "Prevvvvvvvvvvvvvv: %p\n", *prev);
	if (*prev == *next)
	{
		if (PRINT_DEBUG)
			ft_dprintf(2, "[GROUP REMOVE][REMOVE:\'%p\']\n", (*prev)->next);
		free((*prev)->next->nil);
		free((*prev)->next);
		(*prev)->next = NULL;
		g_session->nil->next = g_session->nil;
		g_session->nil->prev = g_session->nil;
	}
	else
	{
		if (PRINT_DEBUG)
			ft_dprintf(2, "[GROUP REMOVE][AT LEAST 1 PROCESS STTOPEED OR IN BACKGROUND!]\n");
		//while (fill != *next)
		//{if (PRINT_DEBUG)
		if (PRINT_DEBUG)
			ft_dprintf(2, "[GROUP REMOVE][REMOVE:\'%p\'] nil= %p prev= %p \n",(*prev)->next, (*prev)->next->nil, *prev);
		free((*prev)->next->nil);
		free((*prev)->next); // remove it process too
			//ft_dprintf(2, " %p ----- %p \n", (*prev), NULL/**next*/);
		(*prev)->next = NULL;
		//}
		//(*prev)->next = *next;
		//(*next)->prev = *prev;
	}
}

/*
** Adds a new group after the head
*/
void			group_push_front(t_group* target)
{
	t_group*	fill;

	if (session_empty())
		group_insert(g_session->nil, g_session->nil, target);
	else
	{
		fill = g_session->nil->next;
		group_insert(g_session->nil, fill, target);
	}
	g_session->groups = g_session->nil->next;
	if (PRINT_DEBUG) {
	ft_dprintf(2, "[GROUP PUSH FRONT][CURR GROUP NOW IS: \'%p\']\n", g_session->groups);
	ft_dprintf(2, "[GROUP PUSH FRONT][PREV=\'%p\'][NEXT=\'%p\']\n", g_session->groups->prev, g_session->groups->next);}
}

/*
** Adds a group before the tail
*/
void			group_push_back(t_group* target)
{
	t_group*	fill;

	if (session_empty())
		group_insert(g_session->nil, g_session->nil, target);
	else
	{
		fill = g_session->nil->prev;
		group_insert(fill, g_session->nil->prev, target);
	}
	g_session->groups = g_session->nil->next;
}

/*
** Removes a group after the head
*/
void			group_pop_front()
{
	t_group*	fill;
	
	if (session_empty())
		return ;
	fill = g_session->nil->next->next;
	//group_remove(&g_session->nil, &g_session->nil->next->next);
	group_remove_v2(&g_session->groups);
	g_session->nil->next = fill;
	g_session->groups = g_session->nil->next;
	if (PRINT_DEBUG)
		ft_dprintf(2, "[GROUP POP FRONT][NEXT GROUP: %p]\n[GROUP POP FRONT][PREV GROUP: %p]\n", g_session->groups->next, g_session->groups->prev);
	if (PRINT_DEBUG)
		ft_dprintf(2, "[GROUP POP FRONT][CURR GROUP IS NOW: \'%p\']\n", g_session->groups);
}

/*
** Removes a group before the tail
*/
void			group_pop_back()
{
	t_group*	fill;

	if (session_empty())
		return ;
	fill = g_session->nil->prev->prev;
	group_remove(&g_session->nil->prev->prev, &g_session->nil->prev);
	g_session->nil->prev = fill;
	g_session->groups = g_session->nil->next;
}
