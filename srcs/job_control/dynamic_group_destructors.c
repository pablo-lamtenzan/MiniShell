/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_group_destructors.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 00:41:09 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 03:40:27 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>
#include <stdlib.h>

void			group_remove(t_group** target)
{
	t_group**	next;
	t_group**	prev;

	next = &(*target)->next;
	prev = &(*target)->prev;
	(*next)->prev = *prev;
	(*prev)->next = *next;
	free(*target);
	*target = NULL;
}

void			group_pop_front()
{
	t_group*	fill;
	
	if (session_empty())
		return ;
	fill = g_session->nil->next->next;
	group_remove(&g_session->groups);
	g_session->nil->next = fill;
	g_session->groups = g_session->nil->next;
}

void			group_pop_back()
{
	t_group*	fill;

	if (session_empty())
		return ;
	fill = g_session->nil->prev->prev;
	group_remove(&g_session->groups);
	g_session->nil->prev = fill;
	g_session->groups = g_session->nil->next;
}
